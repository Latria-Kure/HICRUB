#include "hid_parser.h"
#include "usb_log.h"

uint8_t hid_item_parse(hid_item_t *item, uint8_t report_desc[], uint16_t index)
{
    uint8_t size = 0;
    uint8_t prefix = report_desc[index];
    item->tag = GET_ITEM_TAG(prefix);
    item->type = GET_ITEM_TYPE(prefix);
    size = GET_ITEM_SIZE(prefix);
    if (size == 3) {
        size = 4;
    }
    item->size = size;
    switch (size) {
        case 1:
            item->data = report_desc[index + 1];
            break;
        default:
            break;
    }
    // return new index
    return index + size + 1;
}

void hid_parse(uint8_t report_desc[], uint16_t report_size)
{
    printf("start parse hid report descriptor\r\n");
    uint16_t item_prefix_index = 0;
    hid_item_t item = { 0 };

    while (item_prefix_index < report_size) {
        printf("[%d]", item_prefix_index);
        item_prefix_index = hid_item_parse(&item, report_desc, item_prefix_index);
        switch (item.type) {
            case HID_TAG_MAIN:
                switch (item.tag) {
                    case HID_MAIN_INPUT:
                        printf("INPUT(%d)\r\n", item.data);
                        break;
                    case HID_MAIN_OUTPUT:
                        printf("OUTPUT(%d)\r\n", item.data);
                        break;
                    case HID_MAIN_COLLECTION:
                        printf("COLLECTION(%d)\r\n", item.data);
                        break;
                    case HID_MAIN_FEATURE:
                        printf("FEATURE(%d)\r\n", item.data);
                        break;
                    case HID_MAIN_END_COLLECTION:
                        printf("END_COLLECTION\r\n");
                        break;
                    default:
                        break;
                }
                break;

            case HID_TAG_GLOBAL:
                switch (item.tag) {
                    case HID_GLOBAL_USAGE_PAGE:
                        printf("USAGE_PAGE(%d)\r\n", item.data);
                        break;
                    case HID_GLOBAL_LOGICAL_MINIMUM:
                        printf("LOGICAL_MINIMUM(%d)\r\n", item.data);
                        break;
                    case HID_GLOBAL_LOGICAL_MAXIMUM:
                        printf("LOGICAL_MAXIMUM(%d)\r\n", item.data);
                        break;
                    case HID_GLOBAL_PHYSICAL_MINIMUM:
                        printf("PHYSICAL_MINIMUM(%d)\r\n", item.data);
                        break;
                    case HID_GLOBAL_PHYSICAL_MAXIMUM:
                        printf("PHYSICAL_MAXIMUM(%d)\r\n", item.data);
                        break;
                    case HID_GLOBAL_REPORT_SIZE:
                        printf("REPORT_SIZE(%d)\r\n", item.data);
                        break;
                    case HID_GLOBAL_REPORT_ID:
                        printf("REPORT_ID(%d)\r\n", item.data);
                        break;
                    case HID_GLOBAL_REPORT_COUNT:
                        printf("REPORT_COUNT(%d)\r\n", item.data);
                        break;
                    case HID_GLOBAL_UNIT_EXPONENT:
                    case HID_GLOBAL_UNIT:
                    case HID_GLOBAL_PUSH:
                    case HID_GLOBAL_POP:
                        printf("Not support global tag\r\n");
                        break;
                    default:
                        break;
                }
                break;
            case HID_TAG_LOCAL:
                switch (item.tag) {
                    case HID_LOCAL_USAGE:
                        printf("USAGE(%d)\r\n", item.data);
                        break;
                    case HID_LOCAL_USAGE_MINIMUM:
                        printf("USAGE_MINIMUM(%d)\r\n", item.data);
                        break;
                    case HID_LOCAL_USAGE_MAXIMUM:
                        printf("USAGE_MAXIMUM(%d)\r\n", item.data);
                        break;
                    case HID_LOCAL_DESIGNATOR_INDEX:
                    case HID_LOCAL_DESIGNATOR_MINIMUM:
                    case HID_LOCAL_DESIGNATOR_MAXIMUM:
                    case HID_LOCAL_STRING_INDEX:
                    case HID_LOCAL_STRING_MINIMUM:
                    case HID_LOCAL_STRING_MAXIMUM:
                    case HID_LOCAL_DELIMITER:
                        printf("Not support local tag\r\n");
                        break;
                    default:
                        break;
                }
                break;

            default:
                break;
        }
    }
}