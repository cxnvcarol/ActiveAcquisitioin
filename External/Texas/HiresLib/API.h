/*
 * API.h
 *
 * This module provides C callable APIs for each of the command supported by LightCrafter4500 platform and detailed in the programmer's guide.
 *
 * Copyright (C) 2013 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 *
*/

#ifndef API_H
#define API_H

#include "Common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Total size of all the parameters/payload in bytes */
#define API_MAX_PARAM_SIZE      500

typedef struct
{
    unsigned short firstPixel;
    unsigned short firstLine;
    unsigned short pixelsPerLine;
    unsigned short linesPerFrame;
}rectangle;

typedef struct
{
   unsigned char blstatus;
   unsigned int blVerMajor;
   unsigned int blVerMinor;
   unsigned int blVerPatch;
   unsigned char blASICID;
   unsigned char rsvd[7];
}BootLoaderStaus;

typedef struct
{
	uint08 I2CCmd;			/**< I2C Command number */
	uint08 Read;			/**< Read Command / Write Command */
	uint16 USBCmd;          /**< USB Command number */
	char const *CmdName;	/**< Command Name */
	uint08 *Payload;		/**< Payload buffer */
	uint16 PayloadLen;		/**< Number of bytes in payload */
} API_CommandInfo_t;


typedef void API_DataCallback_t(void *Param, API_CommandInfo_t *CmdInfo);

int LCR_SoftwareReset(hid_device *devHandle);

/* System status commands */
int LCR_GetStatus(unsigned char *pHWStatus, unsigned char *pSysStatus,
                                                    unsigned char *pMainStatus, hid_device *devHandle);
int LCR_GetVersion(unsigned int *pApp_ver, unsigned int *pAPI_ver,
                    unsigned int *pSWConfig_ver, unsigned int *pSeqConfig_ver, hid_device *devHandle);
int LCR_GetFrmwVersion(unsigned int *pFrmwType, char *pFrmwTag, hid_device *devHandle);

//TODO Review: 1. I add 1 param to most of fns declared here. 2. I don't know which fns I actually need. 3. It might make more sense to rewrite this as a class or copy it to the DLPProjector instance!
/* Flash Programing commands */
int LCR_GetBLStatus(BootLoaderStaus *pBL_Status, hid_device *devHandle);
int LCR_SetFlashType(unsigned char Type, hid_device *devHandle);
int LCR_EnterProgrammingMode(hid_device *devHandle);
int LCR_ExitProgrammingMode(hid_device *devHandle);
int LCR_GetProgrammingMode(BOOL *ProgMode, hid_device *devHandle);
int LCR_GetFlashPresent(BOOL *pFlashAtCS0, BOOL *pFlashAtCS1, BOOL *pFlashAtCS2, hid_device *devHandle);
int LCR_GetFlashManID(unsigned short *manID, hid_device *devHandle);
int LCR_GetFlashDevID(unsigned long long *devID, hid_device *devHandle);
int LCR_SetFlashAddr(unsigned int Addr, hid_device *devHandle);
int LCR_SetFlashAddr4Byte(unsigned int Addr, hid_device *devHandle);
int LCR_FlashSectorErase(hid_device *devHandle);
int LCR_SetDownloadSize(unsigned int dataLen, hid_device *devHandle);
int LCR_SetDownloadSize4Byte(unsigned int dataLen, hid_device *devHandle);
int LCR_DownloadData(unsigned char *pByteArray, unsigned int dataLen, hid_device *devHandle);
void LCR_WaitForFlashReady(hid_device *devHandle);
int LCR_CalculateFlashChecksum(hid_device *devHandle);
int LCR_GetFlashChecksum(unsigned int*checksum, hid_device *devHandle);
int LCR_EnableMasterSlave(BOOL MasterEnable, BOOL SlaveEnable, hid_device *devHandle);

int LCR_SetPowerMode(unsigned char, hid_device *devHandle);
int LCR_GetPowerMode(BOOL *Standby, hid_device *devHandle);

int LCR_SetCurtainColor(unsigned int red,unsigned int green, unsigned int blue, hid_device *devHandle);
int LCR_GetCurtainColor(unsigned int *pRed, unsigned int *pGreen, unsigned int *pBlue, hid_device *devHandle);

int LCR_SetDataChannelSwap(unsigned int port, unsigned int swap, hid_device *devHandle);
int LCR_GetDataChannelSwap(unsigned int Port, unsigned int *pSwap, hid_device *devHandle);

int LCR_SetPortClock(unsigned int clock, hid_device *devHandle);
int LCR_GetPortClock(unsigned int *pClock, hid_device *devHandle);

int LCR_SetInputSource(unsigned int source, unsigned int portWidth, hid_device *devHandle);
int LCR_GetInputSource(unsigned int *pSource, unsigned int *portWidth, hid_device *devHandle);

int LCR_SetPixelFormat(unsigned int format, hid_device *devHandle);
int LCR_GetPixelFormat(unsigned int *pFormat, hid_device *devHandle);

int LCR_SetTPGSelect(unsigned int pattern, hid_device *devHandle);
int LCR_GetTPGSelect(unsigned int *pPattern, hid_device *devHandle);

int LCR_SetTPGColor(unsigned short redFG, unsigned short greenFG,
                    unsigned short blueFG, unsigned short redBG,
                    unsigned short greenBG, unsigned short blueBG, hid_device *devHandle);
int LCR_GetTPGColor(unsigned short *pRedFG, unsigned short *pGreenFG,
                    unsigned short *pBlueFG, unsigned short *pRedBG,
                    unsigned short *pGreenBG, unsigned short *pBlueBG, hid_device *devHandle);

int LCR_LoadSplash(unsigned int index, hid_device *devHandle);
int LCR_GetSplashIndex(unsigned int *pIndex, hid_device *devHandle);

int LCR_SetLongAxisImageFlip(BOOL);
BOOL LCR_GetLongAxisImageFlip(hid_device *devHandle);
int LCR_SetShortAxisImageFlip(BOOL);
BOOL LCR_GetShortAxisImageFlip(hid_device *devHandle);

int LCR_SetLedEnables(BOOL SeqCtrl, BOOL Red, BOOL Green, BOOL Blue, hid_device *devHandle);
int LCR_GetLedEnables(BOOL *pSeqCtrl, BOOL *pRed, BOOL *pGreen, BOOL *pBlue, hid_device *devHandle);

int LCR_SetLEDPWMInvert(BOOL invert, hid_device *devHandle);
int LCR_GetLEDPWMInvert(BOOL *inverted, hid_device *devHandle);

int LCR_GetLedCurrents(unsigned char *pRed, unsigned char *pGreen,
                                            unsigned char *pBlue, hid_device *devHandle);
int LCR_SetLedCurrents(unsigned char RedCurrent, unsigned char GreenCurrent,
                                            unsigned char BlueCurrent, hid_device *devHandle);

int LCR_SetGPIOConfig(unsigned int pinNum, BOOL dirOutput,
                                BOOL outTypeOpenDrain, BOOL pinState, hid_device *devHandle);
int LCR_GetGPIOConfig(unsigned int pinNum, BOOL *pDirOutput,
                                BOOL *pOutTypeOpenDrain, BOOL *pState, hid_device *devHandle);

int LCR_SetGeneralPurposeClockOutFreq(unsigned int clkId, BOOL enable, unsigned int clkDivider, hid_device *devHandle);
int LCR_GetGeneralPurposeClockOutFreq(unsigned int clkId, BOOL *pEnabled, unsigned int *pClkDivider, hid_device *devHandle);


int LCR_SetPWMEnable(unsigned int channel, BOOL Enable, hid_device *devHandle);
int LCR_GetPWMEnable(unsigned int channel, BOOL *pEnable, hid_device *devHandle);
int LCR_SetPWMConfig(unsigned int channel, unsigned int pulsePeriod,
                                            unsigned int dutyCycle, hid_device *devHandle);
int LCR_GetPWMConfig(unsigned int channel, unsigned int *pPulsePeriod,
                                            unsigned int *pDutyCycle, hid_device *devHandle);

int LCR_getBatchFileName(unsigned char id, char *batchFileName, hid_device *devHandle);
int LCR_executeBatchFile(unsigned char id, hid_device *devHandle);

int LCR_SetMode(int SLmode, hid_device *devHandle);
int LCR_GetMode(int *pMode, hid_device *devHandle);

int LCR_SetDisplay(rectangle croppedArea, rectangle displayArea, hid_device *devHandle);
int LCR_GetDisplay(rectangle *pCroppedArea, rectangle *pDisplayArea, hid_device *devHandle);

int LCR_SetTrigOutConfig(unsigned int trigOutNum, BOOL invert, short rising, short falling, hid_device *devHandle);
int LCR_GetTrigOutConfig(unsigned int trigOutNum, BOOL *pInvert,short *pRising, short *pFalling, hid_device *devHandle);

int LCR_SetTrigIn1Config(BOOL invert, unsigned int trigDelay, hid_device *devHandle);
int LCR_GetTrigIn1Config(BOOL *pInvert, unsigned int *pTrigDelay, hid_device *devHandle);
int LCR_SetTrigIn1Delay(unsigned int Delay);
int LCR_GetTrigIn1Delay(unsigned int *pDelay);

int LCR_SetTrigIn2Config(BOOL invert, hid_device *devHandle);
int LCR_GetTrigIn2Config(BOOL *pInvert, hid_device *devHandle);

int LCR_SetRedLEDStrobeDelay(short rising, short falling, hid_device *devHandle);
int LCR_SetGreenLEDStrobeDelay(short rising, short falling, hid_device *devHandle);
int LCR_SetBlueLEDStrobeDelay(short rising, short falling, hid_device *devHandle);
int LCR_GetRedLEDStrobeDelay(short *, short *, hid_device *devHandle);
int LCR_GetGreenLEDStrobeDelay(short *, short *, hid_device *devHandle);
int LCR_GetBlueLEDStrobeDelay(short *, short *, hid_device *devHandle);

int LCR_PatternDisplay(int Action, hid_device *devHandle);

int LCR_SetInvertData(BOOL invert, hid_device *devHandle);
int LCR_GetInvertData(BOOL *pInvert, hid_device *devHandle);

int LCR_SetPatternConfig(unsigned int numLutEntries, unsigned int repeat, hid_device *devHandle);
int LCR_GetPatternConfig(unsigned int *pNumLutEntries, BOOL *pRepeat, unsigned int *pNumPatsForTrigOut2, unsigned int *pNumSplash, hid_device *devHandle);

int LCR_ClearPatLut(void);
int LCR_AddToPatLut(int patNum, int ExpUs, BOOL ClearPat, int BitDepth, int LEDSelect, BOOL WaitForTrigger, int DarkTime, BOOL TrigOut2, int SplashIndex, int BitIndex);
int LCR_SendPatLut(hid_device *devHandle);

int LCR_InitPatternMemLoad(BOOL master, unsigned short imageNum, unsigned int size, hid_device *devHandle);
int LCR_pattenMemLoad(BOOL master, unsigned char *pByteArray, int size, hid_device *devHandle);

int LCR_WriteI2CPassThrough(unsigned int port, unsigned int devadd, unsigned char* wdata, unsigned int nwbytes, hid_device *devHandle);
int LCR_ReadI2CPassThrough(unsigned int port, unsigned int devadd, unsigned char* wdata, unsigned int nwbytes, unsigned int nrbytes, unsigned char* rdata, hid_device *devHandle);
int LCR_I2CConfigure(unsigned int port, unsigned int addm, unsigned int clk, hid_device *devHandle);

int LCR_SetPortConfig(unsigned int dataPort,unsigned int pixelClock,unsigned int dataEnable,unsigned int syncSelect, hid_device *devHandle);
int LCR_GetPortConfig(unsigned int *pDataPort,unsigned int *pPixelClock,unsigned int *pDataEnable,unsigned int *pSyncSelect, hid_device *devHandle);

int LCR_enableDebug(hid_device *devHandle);
int LCR_ExecuteRawCommand(uint16 USBCmd, uint08 *Data, int Length, hid_device *devHandle);

int LCR_SetIT6535PowerMode(unsigned int powerMode, hid_device *devHandle);
int LCR_GetIT6535PowerMode(unsigned int *pPowerMode, hid_device *devHandle);

int LCR_SetDMDBlocks(int startBlock, int numBlocks, hid_device *devHandle);
int LCR_GetDMDBlocks(int *startBlock, int *numBlocks, hid_device *devHandle);
int LCR_SetDMDSaverMode(short mode, hid_device *devHandle);
int LCR_GetDMDSaverMode(hid_device *devHandle);

int LCR_ReadErrorCode(unsigned int *pCode, hid_device *devHandle);
int LCR_ReadErrorString(char *errStr, hid_device *devHandle);

int LCR_SetMinLEDPulseWidth(int pulseWidthUS, hid_device *devHandle);
int LCR_GetMinLEDPulseWidth(int *pulseWidthUS, hid_device *devHandle);

int LCR_GetMinPatExposure(int minExposureUS[8], hid_device *devHandle);

void API_SetDataCallback(API_DataCallback_t *Callback, void *Param);
int API_GetI2CCommand(char *command, unsigned char *i2cCommand);
int API_GetUSBCommand(const char *command, uint16 *usbCommand);
int API_GetCommandLength(unsigned char *cmd, int *len);
int API_GetCommandName(unsigned char i2cCommand, const char **command);

#ifdef __cplusplus
}
#endif
#endif // API_H
