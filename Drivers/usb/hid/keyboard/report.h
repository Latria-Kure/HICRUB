#ifndef REPORT_H
#define REPORT_H
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint8_t buffer[8];
    uint8_t idx;
} hid_report_t;

void report_init(void);
void report_press_key(uint8_t keycode);
void report_release_key(uint8_t keycode);
bool send_report(void);

#endif