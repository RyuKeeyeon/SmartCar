/*********************************************************************
    sio.h (Rev 0.94)

    SwitchBox Processing Module

    Copyright(C) Memorylab Ltd.

    Programmed by  Stellar Respree
*********************************************************************/

#ifndef __SIO_H__
#define __SIO_H__

#ifdef __cplusplus
extern "C" {
#endif

SWBOX*	SIO_Init(int bUsePWM);
char*	SIO_GetLastReadLog();

int		SIO_TestWriteLED();
int		SIO_WriteLED(int iLED);
int		SIO_ReadSwitch();

int		SIO_MaxMotorSpeed(int iMaxSpeed);
int		SIO_WriteBLDC(int iSpeed);
int		SIO_BrakeBLDC(int bBrake);

int		SIO_WriteMotor(int iRange, int iSpeed);
int		SIO_WriteServo(int iRange, int iAngle);

int		SIO_ReadIR();
int		SIO_ReadDistUS(int bFront);

char*	SIO_ReadGyro();
char*	SIO_ReadGyroAccel(int* riX, int* riY, int* riZ);
char*	SIO_ReadGyroRotate(int* riX, int* riY, int* riZ);
char*	SIO_ReadGPS();

int     SIO_ActivateLidar(int bOn);
char*	SIO_ReadLidar();

int		SIO_ReadCOM();
int		SIO_WriteCOM();

/*
int		SIO_ReadFIFO(int hFIFO, char* pBuffer);
int		SIO_WriteFIFO(int hFIFO, char* pBuffer, int iLength);
*/

#ifdef __cplusplus
}
#endif

#endif

