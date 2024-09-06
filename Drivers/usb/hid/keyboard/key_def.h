#ifndef KEYDEF_H
#define KEYDEF_H

#include <stdint.h>
// clang-format off
#define  MAX_KEYCODE         0x0072

/* keycode define */
#define  KC_NO               0x00
#define  KC_TRANSPARENT      0x01
#define  KC_A                0x04
#define  KC_B                0x05
#define  KC_C                0x06
#define  KC_D                0x07
#define  KC_E                0x08
#define  KC_F                0x09
#define  KC_G                0x0A
#define  KC_H                0x0B
#define  KC_I                0x0C
#define  KC_J                0x0D
#define  KC_K                0x0E
#define  KC_L                0x0F
#define  KC_M                0x10
#define  KC_N                0x11
#define  KC_O                0x12
#define  KC_P                0x13
#define  KC_Q                0x14
#define  KC_R                0x15
#define  KC_S                0x16
#define  KC_T                0x17
#define  KC_U                0x18
#define  KC_V                0x19
#define  KC_W                0x1A
#define  KC_X                0x1B
#define  KC_Y                0x1C
#define  KC_Z                0x1D
#define  KC_1                0x1E
#define  KC_2                0x1F
#define  KC_3                0x20
#define  KC_4                0x21
#define  KC_5                0x22
#define  KC_6                0x23
#define  KC_7                0x24
#define  KC_8                0x25
#define  KC_9                0x26
#define  KC_0                0x27
#define  KC_ENTER            0x28
#define  KC_ESCAPE           0x29
#define  KC_BACKSPACE        0x2A
#define  KC_TAB              0x2B
#define  KC_SPACE            0x2C
#define  KC_MINUS            0x2D
#define  KC_EQUAL            0x2E
#define  KC_LEFT_BRACKET     0x2F
#define  KC_RIGHT_BRACKET    0x30
#define  KC_BACKSLASH        0x31
#define  KC_NONUS_HASH       0x32
#define  KC_SEMICOLON        0x33
#define  KC_QUOTE            0x34
#define  KC_GRAVE            0x35
#define  KC_COMMA            0x36
#define  KC_DOT              0x37
#define  KC_SLASH            0x38
#define  KC_CAPS_LOCK        0x39
#define  KC_F1               0x3A
#define  KC_F2               0x3B
#define  KC_F3               0x3C
#define  KC_F4               0x3D
#define  KC_F5               0x3E
#define  KC_F6               0x3F
#define  KC_F7               0x40
#define  KC_F8               0x41
#define  KC_F9               0x42
#define  KC_F10              0x43
#define  KC_F11              0x44
#define  KC_F12              0x45
#define  KC_PRINT_SCREEN     0x46
#define  KC_SCROLL_LOCK      0x47
#define  KC_PAUSE            0x48
#define  KC_INSERT           0x49
#define  KC_HOME             0x4A
#define  KC_PAGE_UP          0x4B
#define  KC_DELETE           0x4C
#define  KC_END              0x4D
#define  KC_PAGE_DOWN        0x4E
#define  KC_RIGHT            0x4F
#define  KC_LEFT             0x50
#define  KC_DOWN             0x51
#define  KC_UP               0x52
#define  KC_NUM_LOCK         0x53
#define  KC_KP_SLASH         0x54
#define  KC_KP_ASTERISK      0x55
#define  KC_KP_MINUS         0x56
#define  KC_KP_PLUS          0x57
#define  KC_KP_ENTER         0x58
#define  KC_KP_1             0x59
#define  KC_KP_2             0x5A
#define  KC_KP_3             0x5B
#define  KC_KP_4             0x5C
#define  KC_KP_5             0x5D
#define  KC_KP_6             0x5E
#define  KC_KP_7             0x5F
#define  KC_KP_8             0x60
#define  KC_KP_9             0x61
#define  KC_KP_0             0x62
#define  KC_KP_DOT           0x63
#define  KC_NONUS_BACKSLASH  0x64
#define  KC_APPLICATION      0x65
#define  KC_KB_POWER         0x66
#define  KC_KP_EQUAL         0x67
#define  KC_F13              0x68
#define  KC_F14              0x69
#define  KC_F15              0x6A
#define  KC_LEFT_CTRL        0x6B
#define  KC_LEFT_SHIFT       0x6C
#define  KC_LEFT_ALT         0x6D
#define  KC_LEFT_GUI         0x6E
#define  KC_RIGHT_CTRL       0x6F
#define  KC_RIGHT_SHIFT      0x70
#define  KC_RIGHT_ALT        0x71
#define  KC_RIGHT_GUI        0x72

/* key state define */
#define KEY_RELEASED    0
#define KEY_PRESSED     1
#define KEY_HOLD        2




#endif