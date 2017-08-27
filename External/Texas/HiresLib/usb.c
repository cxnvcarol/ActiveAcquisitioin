/*
 *
 * This module has the wrapper functions to access USB driver functions.
 *
 * Copyright (C) 2013 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 *
*/


#include "usb.h"
#include <stdio.h>
#include <stdlib.h>
#include "API.h"

/**
 * Initialize USB driver
 *
 * @return 0 on success, -ve on failure
 */
int USB_Init(void)
{
    return hid_init();
}

/**
 * Close the USB driver
 *
 * @return  0 on success, -ve on failure
 */
int USB_Exit(void)
{
    return hid_exit();
}

//reusable structs
static hidMessageStruct dummyMsg;
static unsigned char powermode;
static unsigned char dispmode;

/**
 * Writes given bytes to the USB device
 *
 * @return 0 on success, -1 on failure
 */
int USB_Write(uint08 *Data, hid_device *DeviceHandle )
{

    /*    for (int i = 0; i < USB_MIN_PACKET_SIZE; i++)
        printf("0x%x ", Data[i]);
    printf("\n\n");*/
    return hid_write(DeviceHandle, Data, USB_MIN_PACKET_SIZE+1);
}

/**
 * Read one HID packet of data from the USB connection
 *
 * @param Data Pointer to store the read data
 *
 * @return 0 on success -1 on failure
 */
int USB_Read(uint08 *Data, hid_device *DeviceHandle)
{
    if(DeviceHandle == NULL)
        return -1;

    return hid_read_timeout(DeviceHandle, Data, USB_MIN_PACKET_SIZE, 10000);
}

/**
 * Close the USB connection (if any) to the device
 * @return
 */
int USB_Close(hid_device *DeviceHandle)
{
    hid_close(DeviceHandle);
    return 0;
}
