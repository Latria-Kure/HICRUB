#include "keyboard.h"
#include "report.h"

key_t keyboard[MAX_KEYCODE] = { 0 };
uint8_t arrow_kc[4] = { KC_W, KC_A, KC_S, KC_D };
static uint8_t pressed_arrow_keycodes[4];
static uint8_t pressed_arrow_keys_num;
bool kaqi_flag = false;
#define wait_key_release(key, timeout)                                 \
    if (usb_osal_sem_take(key->evnet_tcb.release_sem, timeout) == 0) { \
        return false;                                                  \
    }

bool kaqi_handler(void *argument)
{
    key_t *key = (key_t *)argument;
    report_press_key(KC_I);
    wait_key_release(key, 30);
    report_press_key(KC_D);
    wait_key_release(key, 30);
    report_release_key(KC_I);
    wait_key_release(key, 40);
    report_release_key(KC_D);
    wait_key_release(key, 200);
    return true;
}
void kaqi_release_handler(void *argument)
{
    report_release_key(KC_I);
    report_release_key(KC_D);
}

#define press_arrow_keys()                             \
    for (int i = 0; i < pressed_arrow_keys_num; i++) { \
        report_press_key(pressed_arrow_keycodes[i]);   \
    }

#define release_arrow_keys()                           \
    for (int i = 0; i < pressed_arrow_keys_num; i++) { \
        report_release_key(pressed_arrow_keycodes[i]); \
    }

bool nier_shift_handler(void *argument)
{
    key_t *key = (key_t *)argument;
    for (int i = 0; i < 4; i++) {
        if (keyboard[arrow_kc[i]].physic_key_state == KEY_PRESSED) {
            pressed_arrow_keycodes[pressed_arrow_keys_num++] = arrow_kc[i];
        }
    }
    release_arrow_keys();
    wait_key_release(key, 30);
    press_arrow_keys();
    wait_key_release(key, 30);
    release_arrow_keys();
    wait_key_release(key, 30);
    press_arrow_keys();
    wait_key_release(key, 1000);
    return true;
}

bool space_macro_control_handler(void *argument)
{
    key_t *key = (key_t *)argument;
    if (keyboard[KC_SPACE].evnet_tcb.enable) {
        keyboard[KC_SPACE].evnet_tcb.enable = false;
    } else {
        keyboard[KC_SPACE].evnet_tcb.enable = true;
    }
    return false;
}

void space_macro_control_release_handler(void *argument)
{
    report_release_key(KC_SPACE);
}

bool shift_macro_control_handler(void *argument)
{
    key_t *key = (key_t *)argument;
    if (keyboard[KC_LEFT_SHIFT].evnet_tcb.enable) {
        keyboard[KC_LEFT_SHIFT].evnet_tcb.enable = false;
    } else {
        keyboard[KC_LEFT_SHIFT].evnet_tcb.enable = true;
    }
    return false;
}

void shift_macro_control_release_handler(void *argument)
{
    report_release_key(KC_LEFT_SHIFT);
}

void nier_shift_release_handler(void *argument)
{
    for (int i = 0; i < pressed_arrow_keys_num; i++) {
        key_t *pressed_arrow_key = &keyboard[pressed_arrow_keycodes[i]];
        if (pressed_arrow_key->physic_key_state != pressed_arrow_key->logic_key_state) {
            if (pressed_arrow_key->physic_key_state == KEY_PRESSED) {
                report_press_key(pressed_arrow_keycodes[i]);
            } else {
                report_release_key(pressed_arrow_keycodes[i]);
            }
        }
    }
    memset(pressed_arrow_keycodes, 0, sizeof(pressed_arrow_keycodes));
    pressed_arrow_keys_num = 0;
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
    create_macro(&keyboard[KC_LEFT_SHIFT], nier_shift_handler, nier_shift_release_handler);
    create_macro(&keyboard[KC_F9], space_macro_control_handler, space_macro_control_release_handler);
    create_macro(&keyboard[KC_F10], shift_macro_control_handler, shift_macro_control_release_handler);
}