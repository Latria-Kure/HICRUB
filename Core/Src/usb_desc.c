// clang-format off
#include "usbd_core.h"
#include "usb_hid.h"
#include "usb_desc.h"




const uint8_t ReportDescriptor[HID_REPORT_DESC_SIZE] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
    0x95, 0x05,                    //   REPORT_COUNT (5)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x05, 0x08,                    //   USAGE_PAGE (LEDs)
    0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
    0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x03,                    //   REPORT_SIZE (3)
    0x91, 0x03,                    //   OUTPUT (Cnst,Var,Abs)
    0x95, 0x06,                    //   REPORT_COUNT (6)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0xc0                           // END_COLLECTION
};

const struct usb_device_descriptor kb_device_desc = {
    .bLength            = USB_SIZEOF_DEVICE_DESC,
    .bDescriptorType    = 0x01, // USB_DT_DEVICE
    .bcdUSB             = 0x0200, // USB 2.0
    .bDeviceClass       = 0x00, // Device defined at interface level
    .bDeviceSubClass    = 0x00, // Device defined at interface level
    .bDeviceProtocol    = 0x00, // Device defined at interface level
    .bMaxPacketSize0    = 0x40, // 64 bytes
    .idVendor           = 0x3d41,
    .idProduct          = 0x0721,
    .bcdDevice          = 0x0100,
    .iManufacturer      = 0x01,
    .iProduct           = 0x02,
    .iSerialNumber      = 0x03,
    .bNumConfigurations = 0x01
};

const struct usb_kb_configuration_descriptor kb_config_desc ={
    .config = {
        .bLength             = USB_SIZEOF_CONFIG_DESC,
        .bDescriptorType     = 0x02, // USB_DT_CONFIG
        .wTotalLength        = 0x0029, // 41 bytes
        .bNumInterfaces      = 0x01,
        .bConfigurationValue = 0x01,
        .iConfiguration      = 0x00,
        .bmAttributes        = 0xA0, // Bus powered, Remote Wakeup
        .bMaxPower           = 0x32 // 100 mA
    },
    .interface = {
        .bLength            = USB_SIZEOF_INTERFACE_DESC,
        .bDescriptorType    = 0x04, // USB_DT_INTERFACE
        .bInterfaceNumber   = 0x00,
        .bAlternateSetting  = 0x00,
        .bNumEndpoints      = 0x02,
        .bInterfaceClass    = 0x03, // HID
        .bInterfaceSubClass = 0x01, // Boot
        .bInterfaceProtocol = 0x01, // Keyboard
        .iInterface         = 0x00
    },
    .hid={
        .bLength            = 0x09,
        .bDescriptorType    = 0x21, // HID
        .bcdHID             = 0x0111, // HID 1.11
        .bCountryCode       = 0x00,
        .bNumDescriptors    = 0x01,
        .subdesc={
            {
            .bDescriptorType    = 0x22, // HID_REPORT
            .wDescriptorLength  = sizeof(ReportDescriptor) // 41 bytes 
            }
        },
    },
    .endpoint_in = {
        .bLength            = USB_SIZEOF_ENDPOINT_DESC,
        .bDescriptorType    = 0x05, // USB_DT_ENDPOINT
        .bEndpointAddress   = 0x81, // IN endpoint 1
        .bmAttributes       = 0x03, // Interrupt
        .wMaxPacketSize     = 0x0040,
        .bInterval          = 0x01
    },
    .endpoint_out = {
        .bLength            = USB_SIZEOF_ENDPOINT_DESC,
        .bDescriptorType    = 0x05, // USB_DT_ENDPOINT
        .bEndpointAddress   = 0x01, // OUT endpoint 1
        .bmAttributes       = 0x03, // Interrupt
        .wMaxPacketSize     = 0x0040,
        .bInterval          = 0x01
    }
};

// ����������
const uint8_t MyLangDescr[]
    = { 0x09, 0x04 };
// ������Ϣ
const uint8_t MyManuInfo[] = { 'S', 'T', 'M', '3', '2', 'F', '4', ' ', 'D', 'E', 'V' };
// ��Ʒ��Ϣ
const uint8_t MyProdInfo[] = {'S','T','M','3','2','F','4',' ','U','S','B',' ','H','I','D'};

const uint8_t MySerialNumber [] = {'A','3','5','7','7','6', 'T','P' };


const uint8_t *device_descriptor_callback(uint8_t speed)
{
    return (const uint8_t *)&kb_device_desc;
}

const uint8_t *config_descriptor_callback(uint8_t speed)
{
    return (const uint8_t *)&kb_config_desc;
}

const char *string_descriptor_callback(uint8_t speed, uint8_t index)
{
    switch (index) {
        case 0:
            return (const char *)MyLangDescr;
        case 1:
            return (const char *)MyManuInfo;
        case 2:
            return (const char *)MyProdInfo;
        case 3:
            return (const char *)MySerialNumber;
        default:
            return NULL;
    }
}

const struct usb_descriptor usb_desc = {
    .device_descriptor_callback = device_descriptor_callback,
    .config_descriptor_callback = config_descriptor_callback,
    .string_descriptor_callback = string_descriptor_callback,
};
