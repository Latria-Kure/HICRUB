#include "key_event.h"
#include "report.h"
#include "usbd_core.h"
extern key_t keyboard[MAX_KEYCODE];
extern bool kaqi_flag;

static void key_evnet_task_entry(void *argument)
{
    key_t *key = (key_t *)argument;
    int ret;
    while (1) {
        usb_osal_sem_take(key->evnet_tcb.press_sem, USB_OSAL_WAITING_FOREVER);
        if (key->evnet_tcb.press_handler) {
            // USB_LOG_INFO("press_handler\n");
            key->evnet_tcb.press_handler();
        }
    }
}

void create_macro(key_t *key, key_event_handler_t press_handler, key_event_handler_t release_handler)
{
    key->evnet_tcb.press_handler = press_handler;
    key->evnet_tcb.release_handler = release_handler;
    key->evnet_tcb.press_sem = usb_osal_sem_create(0);
    key->evnet_tcb.release_sem = usb_osal_sem_create(0);
    // assign name with keycode
    char name[16];
    sprintf(name, "key_event_%d", key->keycode);
    // key->evnet_tcb.key_thread = usb_osal_thread_create("key_event", 512, 2, key_evnet_task_entry, key);
    key->evnet_tcb.key_thread = usb_osal_thread_create(name, 512, 2, key_evnet_task_entry, key);
}

bool process_event(uint8_t keycode, key_event_t event)
{
    if (keycode < MAX_KEYCODE) {
        key_t *key = &keyboard[keycode];
        switch (event) {
            case KEY_EVENT_PRESSED:
                if (key->physic_key_state == KEY_RELEASED) {
                    key->physic_key_state = KEY_PRESSED;
                    // if (kaqi_flag && keycode == KC_I) {
                    //     return false;
                    // }
                    if (key->evnet_tcb.key_thread != NULL) {
                        usb_osal_sem_give(key->evnet_tcb.press_sem);
                        return false;
                    } else {
                        return report_press_key(keycode);
                    }
                }
                break;
            case KEY_EVENT_RELEASED:
                // USB_LOG_INFO("release event: %d\n", keycode);
                if (key->physic_key_state == KEY_PRESSED) {
                    key->physic_key_state = KEY_RELEASED;
                    if (key->evnet_tcb.key_thread != NULL) {
                        usb_osal_sem_give(key->evnet_tcb.release_sem);
                        return false;
                    } else {
                        return report_release_key(keycode);
                    }
                }
                break;
            default:
                break;
        }
    }
}