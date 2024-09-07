#include "report.h"
#include "key_def.h"
#include "usbd_core.h"
#include "keyboard.h"

static hid_report_t report;
extern key_t keyboard[MAX_KEYCODE];
usb_osal_mutex_t report_mutex;

void report_init()
{
    memset(report.buffer, 0, sizeof(report.buffer));
    report.idx = 2;
    report_mutex = usb_osal_mutex_create();
}

// return true if buffer changed
bool report_press_key(uint8_t keycode)
{
    usb_osal_mutex_take(report_mutex);
    key_t *key = &keyboard[keycode];
    if (key->logic_key_state == KEY_RELEASED) {
        key->logic_key_state = KEY_PRESSED;
    } else {
        usb_osal_mutex_give(report_mutex);
        return false;
    }
    // modifier keys
    if (keycode >= KC_LEFT_CTRL && keycode <= KC_RIGHT_GUI) {
        report.buffer[0] |= (1 << (keycode - KC_LEFT_CTRL));
        usb_osal_mutex_give(report_mutex);
        return true;
    } else {
        if (report.idx < sizeof(report.buffer)) {
            // USB_LOG_INFO("report_press_key: %d\n", keycode);
            report.buffer[report.idx++] = keycode;
            usb_osal_mutex_give(report_mutex);
            return true;
        } else {
            usb_osal_mutex_give(report_mutex);
            return false;
        }
    }
}

// return true if buffer changed
bool report_release_key(uint8_t keycode)
{
    usb_osal_mutex_take(report_mutex);
    key_t *key = &keyboard[keycode];
    if (key->logic_key_state == KEY_PRESSED) {
        key->logic_key_state = KEY_RELEASED;
    } else {
        usb_osal_mutex_give(report_mutex);
        return false;
    }
    // modifier keys
    if (keycode >= KC_LEFT_CTRL && keycode <= KC_RIGHT_GUI) {
        report.buffer[0] &= ~(1 << (keycode - KC_LEFT_CTRL));
        usb_osal_mutex_give(report_mutex);
        return true;
    } else {
        for (int i = 2; i < sizeof(report.buffer); i++) {
            if (report.buffer[i] == keycode) {
                for (int j = i; j < sizeof(report.buffer) - 1; j++) {
                    report.buffer[j] = report.buffer[j + 1];
                }
                report.idx--;
                report.buffer[report.idx] = 0;
                usb_osal_mutex_give(report_mutex);
                return true;
            }
        }
        usb_osal_mutex_give(report_mutex);
        return false;
    }
}

void send_report()
{
    // usb_osal_mutex_take(report_mutex);
    // printf("[before ep write] report: ");
    // for (int i = 0; i < 8; i++) {
    //     printf("%02X ", report.buffer[i]);
    // }
    // printf("\n");
    usbd_ep_start_write(0, 0x81, report.buffer, 8);
    // printf("[after ep write] report: ");
    // for (int i = 0; i < 8; i++) {
    //     printf("%02X ", report.buffer[i]);
    // }
    // printf("\n");
    // usb_osal_mutex_give(report_mutex);
}