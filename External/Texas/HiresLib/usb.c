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

/***************************************************
*                  GLOBAL VARIABLES //TODO FIX UGLY! -- no multiple devices supported.
****************************************************/
/** USB HID Device handler */
/*
static hid_device *DeviceHandle;//TODO, it should support more than one device connected (at least - and typically - 2)
static BOOL USBConnected = FALSE; //< Device connected status >
*/



static BOOL FakeConnection = FALSE; /**< Simulated connection *///TODO Review... not used, could be removed but I won't touch it now.

/**
 * Enable/disable simulated connection without HW
 *
 * @param enable TRUE = Enable simulated connection
 *               FALSE = Disable simulated connection
 */
void USB_SetFakeConnection(BOOL enable)
{
    FakeConnection = enable;
}

/**
 * Check if USB device is conencted.
 *
 * @return TRUE = USB device is connected, FALSE = USB device is not connected
 //
BOOL USB_IsConnected(hid_device *DeviceHandle)
{
    return TRUE;//TODO.. REMOVE, this is deprecated and now useless!!
}
*/

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
    if(FakeConnection == TRUE)
    {
        memcpy(&dummyMsg, Data + 1, 16);

		if(dummyMsg.head.flags.rw == 0)
		{
			switch(dummyMsg.text.cmd)
			{
				case  0x200: // power mode
					powermode = dummyMsg.text.data[2];
					break;

				case 0x1A1B:
					dispmode = dummyMsg.text.data[2];
					break;
			}
		}
        return 1;
	}


    if(DeviceHandle == NULL)
        return -1;

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
    if(FakeConnection == TRUE)
    {
        switch(dummyMsg.text.cmd)
        {
        case  0x200: // power mode
			dummyMsg.text.data[0] = powermode;
			memcpy(Data, &dummyMsg, 16);
            break;

        case 0x1A1B:
			dummyMsg.text.data[0] = dispmode;
			memcpy(Data, &dummyMsg, 16);
            break;

        default:
            memset(Data, 0, 16);
        }
        return 1;
    }

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
    if(FakeConnection == FALSE)
    {
        hid_close(DeviceHandle);
		//USBConnected = FALSE;
    }
    return 0;
}
