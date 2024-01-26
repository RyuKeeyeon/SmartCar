/*********************************************************************
    hal.h (Rev 1.00)

    HAL Processing Module  (RaspberryPi GPIO/I2C/SPI)

    Copyright(C) Memorylab Ltd.

    Programmed by  Stellar Respree
*********************************************************************/

#ifndef __HAL_H__
#define __HAL_H__

#ifdef __cplusplus
extern "C" {
#endif

int		HAL_Init(int bUsePWM);

int		HAL_WriteGPIO(int iPort, int bOn);
int		HAL_ReadGPIO(int iPort);
int		HAL_WritePWM(int iPort, int iCycle, int iOn);

int		HAL_OpenI2C(char* pDevName, int iID);
int		HAL_CloseI2C();
int		HAL_SelectI2C(int iID);
int		HAL_WriteI2C(char* pReg, int iReg, char* pData, int iData);
int		HAL_ReadI2C(char* pReg, int iReg, char* pData, int iData);

int		HAL_OpenSPI(int iCh, int iSpeed);
int		HAL_CloseSPI();
int		HAL_WriteSPI(char* pBuffer, int iLength);
int		HAL_ReadSPI(char* pBuffer, int iLength);

int		HAL_OpenCOM(int iComID, int iSpeed, int iOption);
int		HAL_CloseCOM(int iComID);
int		HAL_SelectCOM(int iComID);
int		HAL_WriteCOM(int iComID, char* pBuffer, int iLength);
int		HAL_ReadCOM(int iComID, char* pBuffer, int iLength);

int		HAL_ReadOneWire(int iPort);

int		HAL_OpenFIFO(char* pName, int iMode);
int		HAL_CloseFIFO(int hHandle);
int		HAL_WriteFIFO(int hHandle, char* pBuffer, int iLength);
int		HAL_ReadFIFO(int hHandle, char* pBuffer, int iLength);

int		HAL_GetLocalTime();
int		HAL_GetTickCount(int bClear);
int		HAL_WaitTickCount(int iNS);

#ifdef __cplusplus
}
#endif

#endif
