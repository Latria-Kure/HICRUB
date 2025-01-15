#ifndef HID_PARSER_H
#define HID_PARSER_H
#include <stdint.h>
/* HID Host Driver. Only support keyboard, mouse and joystick */
#define GET_ITEM_TAG(prefix)  (((prefix) & 0xF0) >> 4)
#define GET_ITEM_TYPE(prefix) (((prefix) & 0x0C) >> 2)
#define GET_ITEM_SIZE(prefix) ((prefix) & 0x03)

/* HID tag define */
#define HID_TAG_MAIN   0x00
#define HID_TAG_GLOBAL 0x01
#define HID_TAG_LOCAL  0x02

/* HID main item define */
#define HID_MAIN_INPUT          0x08
#define HID_MAIN_OUTPUT         0x09
#define HID_MAIN_COLLECTION     0x0A
#define HID_MAIN_FEATURE        0x0B
#define HID_MAIN_END_COLLECTION 0x0C

/* HID global item define */
#define HID_GLOBAL_USAGE_PAGE       0x00
#define HID_GLOBAL_LOGICAL_MINIMUM  0x01
#define HID_GLOBAL_LOGICAL_MAXIMUM  0x02
#define HID_GLOBAL_PHYSICAL_MINIMUM 0x03
#define HID_GLOBAL_PHYSICAL_MAXIMUM 0x04
#define HID_GLOBAL_UNIT_EXPONENT    0x05
#define HID_GLOBAL_UNIT             0x06
#define HID_GLOBAL_REPORT_SIZE      0x07
#define HID_GLOBAL_REPORT_ID        0x08
#define HID_GLOBAL_REPORT_COUNT     0x09
#define HID_GLOBAL_PUSH             0x0A
#define HID_GLOBAL_POP              0x0B

/* HID local item define */
#define HID_LOCAL_USAGE              0x00
#define HID_LOCAL_USAGE_MINIMUM      0x01
#define HID_LOCAL_USAGE_MAXIMUM      0x02
#define HID_LOCAL_DESIGNATOR_INDEX   0x03
#define HID_LOCAL_DESIGNATOR_MINIMUM 0x04
#define HID_LOCAL_DESIGNATOR_MAXIMUM 0x05
#define HID_LOCAL_STRING_INDEX       0x07
#define HID_LOCAL_STRING_MINIMUM     0x08
#define HID_LOCAL_STRING_MAXIMUM     0x09
#define HID_LOCAL_DELIMITER          0x0A

/* HID report type define */
#define HID_REPORT_TYPE_INPUT   0x01
#define HID_REPORT_TYPE_OUTPUT  0x02
#define HID_REPORT_TYPE_FEATURE 0x03

typedef struct {
    uint8_t tag;
    uint8_t size;
    uint8_t type;
    uint8_t data;
} hid_item_t;

void hid_parse(uint8_t report_desc[], uint16_t report_size);
uint8_t hid_item_parse(hid_item_t *item, uint8_t report_desc[], uint16_t index);

#endif