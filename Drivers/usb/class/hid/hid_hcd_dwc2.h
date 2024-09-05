#ifndef HID_HCD_DWC2_H
#define HID_HCD_DWC2_H
#include "usbh_core.h"
#include "usb_hc.h"
#include "usbh_hub.h"
#include "usb_dwc2_reg.h"
#include "usbh_hid.h"

struct hid_user_data {
    usb_osal_thread_t hid_thread;
    usb_osal_thread_t key_thread;
};

#endif