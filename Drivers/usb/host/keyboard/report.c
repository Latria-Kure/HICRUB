#include "report.h"
#include "key_def.h"
#include "usbd_core.h"
#include "keyboard.h"

static hid_report_t report;
extern key_t keyboard[MAX_KEYCODE];
usb_osal_mutex_t report_mutex;
bool changed = false;

void report_init()
{
    memset(report.buffer, 0, sizeof(report.buffer));
    report.idx = 2;
    report_mutex = usb_osal_mutex_create();
}

// return true if buffer changed
void report_press_key(uint8_t keycode)
{
    usb_osal_mutex_take(report_mutex);
    key_t *key = &keyboard[keycode];
    if (key->logic_key_state == KEY_PRESSED) {
        usb_osal_mutex_give(report_mutex);
        return;
    }
    // modifier keys
    if (keycode >= KC_LEFT_CTRL && keycode <= KC_RIGHT_GUI) {
        report.buffer[0] |= (1 << (keycode - KC_LEFT_CTRL));
        key->logic_key_state = KEY_PRESSED;
        changed = true;
        usb_osal_mutex_give(report_mutex);
        return;
    } else {
        if (report.idx < sizeof(report.buffer)) {
            // USB_LOG_INFO("report_press_key: %d\n", keycode);
            report.buffer[report.idx++] = keycode;
            key->logic_key_state = KEY_PRESSED;
            changed = true;
        }
        usb_osal_mutex_give(report_mutex);
        return;
    }
}

// return true if buffer changed
void report_release_key(uint8_t keycode)
{
    usb_osal_mutex_take(report_mutex);
    key_t *key = &keyboard[keycode];
    if (key->logic_key_state == KEY_RELEASED) {
        usb_osal_mutex_give(report_mutex);
        return;
    }
    // modifier keys
    if (keycode >= KC_LEFT_CTRL && keycode <= KC_RIGHT_GUI) {
        report.buffer[0] &= ~(1 << (keycode - KC_LEFT_CTRL));
        key->logic_key_state = KEY_RELEASED;
        changed = true;
        usb_osal_mutex_give(report_mutex);
        return;
    } else {
        for (int i = 2; i < sizeof(report.buffer); i++) {
            if (report.buffer[i] == keycode) {
                for (int j = i; j < sizeof(report.buffer) - 1; j++) {
                    report.buffer[j] = report.buffer[j + 1];
                }
                report.idx--;
                report.buffer[report.idx] = 0;
                key->logic_key_state = KEY_RELEASED;
                changed = true;
                usb_osal_mutex_give(report_mutex);
                return;
            }
        }
        usb_osal_mutex_give(report_mutex);
        return;
    }
}

bool send_report()
{
    usb_osal_mutex_take(report_mutex);
    bool ret = false;
    // printf("[before ep write] report: ");
    // for (int i = 0; i < 8; i++) {
    //     printf("%02X ", report.buffer[i]);
    // }
    // printf("\n");
    if (changed) {
        usbd_ep_start_write(0, 0x81, report.buffer, 8);
        changed = false;
        ret = true;
    }
    // printf("[after ep write] report: ");
    // for (int i = 0; i < 8; i++) {
    //     printf("%02X ", report.buffer[i]);
    // }
    // printf("\n");
    usb_osal_mutex_give(report_mutex);
    return ret;
}