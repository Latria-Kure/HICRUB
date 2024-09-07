#ifndef KEY_EVNET_H
#define KEY_EVNET_H
#include "usb_osal.h"
#include "key_def.h"

// clang-format off
/* key event define */
typedef uint8_t key_event_t;
#define KEY_EVENT_RELEASED      0
#define KEY_EVENT_PRESSED       1
#define KEY_EVENT_HOLD          2
// clang-format on

typedef void (*key_event_handler_t)(void);

typedef struct {
    key_event_handler_t press_handler;
    key_event_handler_t release_handler;
    usb_osal_thread_t key_thread;
    usb_osal_sem_t press_sem;
    usb_osal_sem_t release_sem;
} key_evnent_tcb_t;

typedef struct {
    uint8_t keycode;
    uint8_t physic_key_state;
    uint8_t logic_key_state;
    key_evnent_tcb_t evnet_tcb;
} key_t;

void create_macro(key_t *key, key_event_handler_t handler, key_event_handler_t release_handler);
bool process_event(uint8_t keycode, key_event_t event);

#endif