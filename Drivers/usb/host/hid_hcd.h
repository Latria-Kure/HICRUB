#ifndef HID_HCD_H
#define HID_HCD_H
#include "usbh_core.h"

struct hid_user_data {
    usb_osal_thread_t hid_thread;
    usb_osal_thread_t key_thread;
    usb_osal_thread_t report_thread;
};

#endif