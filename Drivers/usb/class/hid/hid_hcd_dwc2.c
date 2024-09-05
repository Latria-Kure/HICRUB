
#include "hid_hcd_dwc2.h"
#include "usbd_core.h"

uint8_t hid_kb_buffer[8] = { 0 };
uint8_t hid_kb_buffer_raw[8] = { 0 };
static usb_osal_sem_t hid_sem;

void hid_in_callback(void)
{
    if (memcmp(hid_kb_buffer, hid_kb_buffer_raw, 8) != 0) {
        memcpy(hid_kb_buffer, hid_kb_buffer_raw, 8);
        usb_osal_sem_give(hid_sem);
    }
}

void hid_key_task(void)
{
    int ret;
    while (1) {
        ret = usb_osal_sem_take(hid_sem, USB_OSAL_WAITING_FOREVER);
        if (ret == 0) {
            usbd_ep_start_write(0, 0x81, hid_kb_buffer, 8);
        }
    }
}

void hid_urb_fill(struct usbh_hid *hid_clas, uint8_t *buffer, uint32_t buflen)
{
    // fill in urb
    struct usbh_urb *urb = &hid_clas->intin_urb;
    urb->hport = hid_clas->hport;
    urb->ep = hid_clas->intin;
    urb->data_toggle = 1;
    urb->interval = hid_clas->intin->bInterval;
    urb->transfer_buffer = buffer;
    urb->transfer_buffer_length = buflen;
    urb->timeout = 0xffffffff;
    urb->errorcode = 0;
    urb->complete = hid_in_callback;
}

void hid_in_poll_thread(void *argument)
{
    struct usbh_hid *hid_class = (struct usbh_hid *)argument;
    struct usbh_urb *urb = &hid_class->intin_urb;
    while (1) {
        urb->timeout = 0xffffffff;
        usbh_submit_urb(urb);
        usb_osal_msleep(1);
    }
}

void usbh_hid_run(struct usbh_hid *hid_class)
{
    USB_LOG_INFO("interface number: %d\r\n", hid_class->intf);
    USB_LOG_INFO("HID report descriptor: \r\n");
    for (int i = 0; i < 256; i++) {
        printf("%02x ", hid_class->report_desc[i]);
    }
    if (hid_class->intf == 0) {
        hid_urb_fill(hid_class, hid_kb_buffer_raw, 8);
        struct hid_user_data *user_data = malloc(sizeof(struct hid_user_data));
        hid_sem = usb_osal_sem_create(0);
        user_data->hid_thread = usb_osal_thread_create("hid_in_poll_thread", 512, 2, hid_in_poll_thread, hid_class);
        user_data->key_thread = usb_osal_thread_create("hid_key_task", 512, 3, hid_key_task, NULL);
        hid_class->user_data = user_data;
    }
}