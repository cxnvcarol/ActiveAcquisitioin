/**
 * This module has the wrapper functions to access USB driver functions.
 *
 * Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 *
*/

#ifndef USB_H
#define USB_H

#include "common.h"
#include "hidapi.h"

#ifdef __cplusplus
extern "C" {
#endif


#define USB_MIN_PACKET_SIZE 64
#define USB_MAX_PACKET_SIZE 64

#define MY_VID					0x0451
#define MY_PID					0xC900
#define HID_MESSAGE_MAX_SIZE    512

typedef struct
{
    struct
    {
        struct
        {
            unsigned char dest		:3; /* 0 - ProjCtrl; 1 - RFC; 7 - Debugmsg */
            unsigned char reserved	:2;
            unsigned char nack		:1; /* Command Handler Error */
            unsigned char reply	    :1; /* Host wants a reply from device */
            unsigned char rw		:1; /* Write = 0; Read = 1 */
        }flags;
        unsigned char seq;
        unsigned short length;
    }head;
    union
    {
        unsigned short cmd;
        unsigned char data[HID_MESSAGE_MAX_SIZE];
    }text;
}hidMessageStruct;
/*hid_device *DeviceHandle
typedef struct{
	hid_device *DeviceHandle;
	BOOL USBConnected;
}hidCommDevice;//communication with HID detected device.
*/

void USB_SetFakeConnection(BOOL enable);//todo remove, deprecated and useless for this application.
//BOOL USB_IsConnected(hid_device *DeviceHandle);
int USB_Write(uint08 *Data, hid_device *DeviceHandle);
int USB_Read(uint08 *Data, hid_device *DeviceHandle);
int USB_Close(hid_device *DeviceHandle);
int USB_Init();
int USB_Exit();


#ifdef __cplusplus
}
#endif


#endif //USB_H
