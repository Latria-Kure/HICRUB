
#include "hid_hcd_dwc2.h"
#include "usbd_core.h"
#include "report.h"
#include "keyboard.h"

uint8_t report_current[8] = { 0 };
uint8_t report_previous[8] = { 0 };
static usb_osal_sem_t hid_sem;
static usb_osal_sem_t report_sem;

void hid_in_callback(void)
{
    if (memcmp(report_previous, report_current, 8) != 0) {
        usb_osal_sem_give(hid_sem);
    }
}

void hid_event_detect_task(void)
{
    int ret;
    bool changed;
    while (1) {
        ret = usb_osal_sem_take(hid_sem, USB_OSAL_WAITING_FOREVER);
        if (ret == 0) {
            changed = false;
            // modifier keys
            if (report_current[0] != report_previous[0]) {
                // find the changed bit
                for (int i = 0; i < 8; i++) {
                    if ((report_current[0] & (1 << i)) != (report_previous[0] & (1 << i))) {
                        if (report_current[0] & (1 << i)) {
                            // changed |= report_press_key(KC_LEFT_CTRL + i);
                            changed |= process_event((uint8_t)(KC_LEFT_CTRL + i), KEY_EVENT_PRESSED);
                        } else {
                            // changed |= report_release_key(KC_LEFT_CTRL + i);
                            changed |= process_event((uint8_t)(KC_LEFT_CTRL + i), KEY_EVENT_RELEASED);
                        }
                    }
                }
            }

            // normal keys
            // Identify pressed keys
            for (int i = 2; i < 8; i++) {
                if (report_current[i] != 0) {
                    bool found = false;
                    for (int j = 2; j < 8; j++) {
                        if (report_current[i] == report_previous[j]) {
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        // changed |= report_press_key(report_current[i]);
                        changed |= process_event(report_current[i], KEY_EVENT_PRESSED);
                    }
                }
            }

            // Identify released keys
            for (int i = 2; i < 8; i++) {
                if (report_previous[i] != 0) {
                    bool found = false;
                    for (int j = 2; j < 8; j++) {
                        if (report_previous[i] == report_current[j]) {
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        // changed |= report_release_key(report_previous[i]);
                        changed |= process_event(report_previous[i], KEY_EVENT_RELEASED);
                    }
                }
            }
            if (changed) {
                send_report();
            }
            memcpy(report_previous, report_current, 8);
        }
    }
}

// void report_task(void)
// {
//     int ret;
//     while (1) {
//         ret = usb_osal_sem_take(report_sem, USB_OSAL_WAITING_FOREVER);
//         if (ret == 0) {
//             send_report();
//         }
//     }
// }

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
        report_init();
        keyboard_init();
        hid_urb_fill(hid_class, report_current, 8);
        struct hid_user_data *user_data = malloc(sizeof(struct hid_user_data));
        hid_sem = usb_osal_sem_create(0);
        report_sem = usb_osal_sem_create(0);
        user_data->hid_thread = usb_osal_thread_create("hid_in_poll_thread", 512, 5, hid_in_poll_thread, hid_class);
        user_data->key_thread = usb_osal_thread_create("hid_event_detect_task", 512, 3, hid_event_detect_task, NULL);
        // user_data->report_thread = usb_osal_thread_create("report_task", 256, 2, report_task, NULL);
        hid_class->user_data = user_data;
    }
}