#include "ds4_lib.h"

static usb_device_entry ds4_dev_entries[2];
static u8 ATTRIBUTE_ALIGN(32) ds4_data[8];

static ds4_state ds4_state_data;
static s32 ds4_fd;

static int ds4_reading = 0;

float byte_to_norm(u8 n) {
    return ((float)n - 127.5f) / 127.5f;
}

s32 ds4_callback(s32 result, void *usrdata) {
    ds4_state_data.leftX = byte_to_norm(ds4_data[1]);
    ds4_state_data.leftY = byte_to_norm(ds4_data[2]);
    ds4_state_data.rightX = byte_to_norm(ds4_data[3]);
    ds4_state_data.rightY = byte_to_norm(ds4_data[4]);

    int button_fields1 = ds4_data[5];
    ds4_state_data.square = button_fields1 & 0x10;
    ds4_state_data.cross = button_fields1 & 0x20;
    ds4_state_data.circle = button_fields1 & 0x40;
    ds4_state_data.triangle = button_fields1 & 0x80;

    int button_fields2 = ds4_data[6];
    ds4_state_data.l1 = button_fields2 & 0x01;
    ds4_state_data.r1 = button_fields2 & 0x02;
    ds4_state_data.l2 = button_fields2 & 0x04;
    ds4_state_data.r2 = button_fields2 & 0x08;
    
    ds4_state_data.select = button_fields2 & 0x10;
    ds4_state_data.start = button_fields2 & 0x20;

    ds4_reading = 0;

    return 0;
}

s32 ds4_removal_callback(s32 result, void *usrdata) {
    ds4_state_data.exists = 0;

    return 0;
}

ds4_state get_ds4_state() {
    return ds4_state_data;
}

void start_ds4() {
    ds4_state_data.exists = 0;
}

void update_ds4() {
    if (!ds4_state_data.exists) {
        u8 dev_count;
        USB_GetDeviceList(ds4_dev_entries, 2, USB_CLASS_HID, &dev_count);

        for (int i = 0; i < dev_count; i++) {
            ds4_state_data.exists = 1;

            usb_device_entry* dev_entry = &ds4_dev_entries[i];
            USB_OpenDevice(dev_entry->device_id, 0x054C, 0x05C4, &ds4_fd);

            USB_DeviceRemovalNotifyAsync(ds4_fd, &ds4_removal_callback, NULL);

            break;
        }
    }

    if (ds4_state_data.exists && !ds4_reading) {
        ds4_reading = 1;

        USB_WriteIntrMsgAsync(ds4_fd, USB_ENDPOINT_IN, sizeof(ds4_data), ds4_data, ds4_callback, NULL);
    }
}

ds4_state _Z13get_ds4_statev() {
    return get_ds4_state();
}

void _Z9start_ds4v() {
    start_ds4();
}

void _Z10update_ds4v() {
    update_ds4();
}