#include "report.h"
#include "key_def.h"
#include "usbd_core.h"

static hid_report_t report;

void report_init()
{
    memset(report.buffer, 0, sizeof(report.buffer));
    report.idx = 2;
}

// return true if buffer changed
bool report_press_key(uint8_t keycode)
{
    // modifier keys
    if (keycode >= KC_LEFT_CTRL && keycode <= KC_RIGHT_GUI) {
        report.buffer[0] |= (1 << (keycode - KC_LEFT_CTRL));
        return true;
    } else {
        if (report.idx < sizeof(report.buffer)) {
            report.buffer[report.idx++] = keycode;
            return true;
        } else {
            return false;
        }
    }
}

// return true if buffer changed
bool report_release_key(uint8_t keycode)
{
    // modifier keys
    if (keycode >= KC_LEFT_CTRL && keycode <= KC_RIGHT_GUI) {
        report.buffer[0] &= ~(1 << (keycode - KC_LEFT_CTRL));
        return true;
    } else {
        for (int i = 2; i < sizeof(report.buffer); i++) {
            if (report.buffer[i] == keycode) {
                report.buffer[i] = 0;
                // move the rest keys forward
                for (int j = i; j < sizeof(report.buffer) - 1; j++) {
                    report.buffer[j] = report.buffer[j + 1];
                }
                report.idx--;
                return true;
            }
        }
        return false;
    }
}

void send_report()
{
    usbd_ep_start_write(0, 0x81, report.buffer, 8);
}