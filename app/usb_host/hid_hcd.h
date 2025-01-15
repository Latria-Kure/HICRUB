#ifndef HID_HCD_H
#define HID_HCD_H
#include "usbh_core.h"

#define CONFIG_USBHOST_MAX_HID_DEV 3

struct hid_dev_manager {
    struct usbh_hid *hid_dev[CONFIG_USBHOST_MAX_HID_DEV];
    uint8_t dev_num;
};

struct hid_user_data {
    usb_osal_thread_t hid_thread;
    usb_osal_thread_t key_thread;
    usb_osal_thread_t report_thread;
};

#endif