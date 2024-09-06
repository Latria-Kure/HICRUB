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
    uint8_t keycode;
    uint8_t key_state;
} key_t;

#endif