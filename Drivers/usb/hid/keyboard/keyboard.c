#include "keyboard.h"
#include "report.h"

key_t keyboard[MAX_KEYCODE] = { 0 };
bool kaqi_flag = false;

void kaqi_handler(void)
{
    report_press_key(KC_I);
    send_report();
    usb_osal_msleep(20);
    report_press_key(KC_D);
    send_report();
    usb_osal_msleep(20);
    report_release_key(KC_I);
    send_report();
    usb_osal_msleep(20);
    report_release_key(KC_D);
    send_report();
    usb_osal_msleep(200);
    kaqi_flag = false;
    if (keyboard[KC_I].physic_key_state == KEY_PRESSED) {
        report_press_key(KC_I);
        send_report();
    }
}
void kaqi_release_handler(void)
{
}

void forawrd_press_handler(void)
{
    if (kaqi_flag) {
    } else {
        report_press_key(KC_I);
        // send_report();
    }
}

void forawrd_release_handler(void)
{
    report_release_key(KC_I);
    send_report();
}

void keyboard_init(void)
{
    for (int i = 0; i < MAX_KEYCODE; i++) {
        keyboard[i].keycode = i;
        keyboard[i].physic_key_state = KEY_RELEASED;
        keyboard[i].logic_key_state = KEY_RELEASED;
        keyboard[i].evnet_tcb.press_handler = NULL;
        keyboard[i].evnet_tcb.release_handler = NULL;
        keyboard[i].evnet_tcb.key_thread = NULL;
        keyboard[i].evnet_tcb.press_sem = NULL;
        keyboard[i].evnet_tcb.release_sem = NULL;
    }
    create_macro(&keyboard[KC_SPACE], kaqi_handler, kaqi_release_handler);
    // create_macro(&keyboard[KC_I], forawrd_press_handler, forawrd_release_handler);
}