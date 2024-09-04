#ifndef USB_DESC_H
#define USB_DESC_H

#define HID_REPORT_DESC_SIZE 63

struct usb_kb_configuration_descriptor {
    struct usb_configuration_descriptor config;
    struct usb_interface_descriptor interface;
    struct usb_hid_descriptor hid;
    struct usb_endpoint_descriptor endpoint_in;
    struct usb_endpoint_descriptor endpoint_out;
} __attribute__((packed));

#endif