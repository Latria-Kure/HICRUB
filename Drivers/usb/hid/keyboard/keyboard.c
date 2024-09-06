#include "keyboard.h"

static key_t keyboard[MAX_KEYCODE] = { 0 };

void keyboard_init(void)
{
    for (int i = 0; i < MAX_KEYCODE; i++) {
        keyboard[i].keycode = i;
        keyboard[i].key_state = KEY_RELEASED;
    }
}