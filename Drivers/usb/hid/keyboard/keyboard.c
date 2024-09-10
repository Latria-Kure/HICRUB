#include "keyboard.h"
#include "report.h"

key_t keyboard[MAX_KEYCODE] = { 0 };
bool kaqi_flag = false;
#define wait_key_release(key, timeout)                                 \
    if (usb_osal_sem_take(key->evnet_tcb.release_sem, timeout) == 0) { \
        return false;                                                  \
    }

bool kaqi_handler(void *argument)
{
    key_t *key = (key_t *)argument;
    report_press_key(KC_I);
    wait_key_release(key, 20);
    report_press_key(KC_D);
    wait_key_release(key, 10);
    report_release_key(KC_I);
    wait_key_release(key, 20);
    report_release_key(KC_D);
    wait_key_release(key, 200);
    return true;
}
void kaqi_release_handler(void *argument)
{
    report_release_key(KC_I);
    report_release_key(KC_D);
}

bool supper_drift_handler(void *argument)
{
    key_t *key = (key_t *)argument;
    report_press_key(KC_I);
    wait_key_release(key, 20);
    report_press_key(KC_D);
    wait_key_release(key, 10);
    report_release_key(KC_I);
    wait_key_release(key, 50);
    report_release_key(KC_D);
    wait_key_release(key, 300);
    return true;
}

void supper_drift_release_handler(void *argument)
{
    report_release_key(KC_I);
    report_release_key(KC_D);
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
    // create_macro(&keyboard[KC_E], supper_drift_handler, supper_drift_release_handler);
}