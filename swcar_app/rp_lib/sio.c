/*********************************************************************
    sio.c (Rev 0.94)

    SwitchBox Processing Module

    Copyright(C)

    Programmed by  Stellar Respree
*********************************************************************/

#include <global.h>
#include "local.h"
#include "hal_type.h"
#include "hal.h"
#include "sw_type.h"
#include "sio.h"


// -------------------------------------------------------------------
//	Main Memory
// -------------------------------------------------------------------
SWBOX	xMM;
SWBOX*	pMM = &xMM;

char*	m_pReadLog;


#define COM_TTYAMA1			1
#define COM_GPS				1
#define COM_LIDAR			2
#define I2C_ID_LEDMUX		0x20
#define I2C_ID_GYRO			0x68

#define SPI_CH				0
#define SPI_SPEED			(50 * 1000)
/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
SWBOX*	SIO_Init(int bUsePWM)
{
	char	cData[32];

	memset(pMM, 0, sizeof(SWBOX));

#ifdef RASPBIAN
	HAL_Init(bUsePWM);

	// i2cdetect -l
	// i2cdetect -y 1
	HAL_OpenI2C(NULL, I2C_ID_LEDMUX);

///	HAL_OpenSPI(SPI_CH, SPI_SPEED);

	pMM->hPWM		= HAL_OpenFIFO(FIFO_PWM, FIFO_MODE);

	pMM->hCOM_GPS	= HAL_OpenCOM(COM_GPS  ,   9600, 0);
	pMM->hCOM_LDA	= HAL_OpenCOM(COM_LIDAR, 115200, 0);
#endif

	return (pMM);
}


/// ------------------------------------------------------------------
/// Interface Function
/// ----------------------------------------------n--------------------
char*	SIO_GetLastReadLog()
{
	return (m_pReadLog);
}


/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int		sio_UpdateTestLED()
{
	static	int		iCount= 0;

	iCount;

	pMM->iLED[0] = iCount % 10;
	pMM->iLED[1] = iCount / 10;

	iCount = (iCount + 1) % 100;

	return (TRUE);
}


/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int		SIO_TestWriteLED()
{
	int		i;

	sio_UpdateTestLED();

	i = pMM->iLED[1] * 10 + pMM->iLED[0];

	SIO_WriteLED(i);
}


/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int		SIO_WriteLED(int iLED)
{
	static	int		bInit = FALSE;
	static	char	sLog[256];

	int		iAddr;
	int		iData;
	char*	pAddr = (char*)&iAddr;
	char*	pData = (char*)&iData;

	HAL_SelectI2C(I2C_ID_LEDMUX);

	if ( bInit == FALSE ) {
		iAddr = 0;
		iData = 0;
		HAL_WriteI2C(pAddr, 1, pData, 1);

		bInit = TRUE;
	}
	
	pMM->iLED[0] = iLED % 100 % 10;
	pMM->iLED[1] = iLED % 100 / 10;

	iAddr = 9;
	iData = pMM->iLED[0] | 0x20;		// Enable LE0
	HAL_WriteI2C(pAddr, 1, pData, 1);
	iData = pMM->iLED[0] | 0x30;		// Close LE0
	HAL_WriteI2C(pAddr, 1, pData, 1);

	iData = pMM->iLED[1] | 0x10;		// Enable LE1
	HAL_WriteI2C(pAddr, 1, pData, 1);
	iData = pMM->iLED[1] | 0x30;		// Close LE1
	HAL_WriteI2C(pAddr, 1, pData, 1);

	sprintf(sLog, "[%d%d]", pMM->iLED[1] , pMM->iLED[0]);

	m_pReadLog = sLog;

	iData = pMM->iLED[1] * 10 + pMM->iLED[0];

	return (iData);
}


/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int		SIO_ReadSwitch()
{
	static	char	sLog[256];

	int		i;
	int		j;

	pMM->iSW[0] = HAL_ReadGPIO(CAR_SW0);
	pMM->iSW[1] = HAL_ReadGPIO(CAR_SW1);
	pMM->iSW[2] = HAL_ReadGPIO(CAR_SW2);
#ifdef USE_LIDAR
#else
	pMM->iSW[3] = HAL_ReadGPIO(CAR_SW3);
#endif

	j = 0;
	sLog[0] = '[';

	for ( int i = 0 ; i < MAX_SW ; i++ ) {
		sLog[++j] = (pMM->iSW[i] == 1) ? 'O' : '.';
	}

	sLog[++j] = ']';
	sLog[++j] = 0;

	m_pReadLog = sLog;

	i  = 0;
	i += (pMM->iSW[0] * 1);
	i += (pMM->iSW[1] * 2);
	i += (pMM->iSW[2] * 4);
	i += (pMM->iSW[3] * 8);

	return (i);
}


int		m_iMaxSpeed = 100;
/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int		SIO_MaxMotorSpeed(int iMaxSpeed)
{
	m_iMaxSpeed = iMaxSpeed;
}


/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int		SIO_WriteBLDC(int iSpeed)
{
	static	char	sLog[256];

	int		bDir	= (iSpeed >= 0) ? TRUE : FALSE;
	int		iPWM;

	HAL_WriteGPIO(CAR_BLDC_DIR, bDir);

	if ( iSpeed < 0 )				iSpeed = -iSpeed;
	if ( iSpeed > m_iMaxSpeed )		iSpeed = m_iMaxSpeed;

	iPWM	= iSpeed * 5;
	iPWM	= 1000 - iPWM;

	if ( pMM->hPWM == 0 ) {
		HAL_WritePWM(CAR_MOTOR_PM, 0, iSpeed);
	}
	else {
		char	sFIFO[256];

		sprintf(sFIFO, "M %d\n", iSpeed);
		HAL_WriteFIFO(pMM->hPWM, sFIFO, strlen(sFIFO));
	}

	sprintf(sLog, "[%6d:%6d]", iSpeed, iPWM);

	m_pReadLog = sLog;

	return (iSpeed);
}


/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int		SIO_BrakeBLDC(int bBrake)
{
	HAL_WriteGPIO(CAR_BLDC_BRAKE, bBrake);

	return (TRUE);
}


#define PWM_MOTOR_CENTER	1500
#define PWM_MOTOR_DEADZONE	  80
#define PWM_SERVO_CENTER	1500
/// ------------------------------------------------------------------
/// Interface Function
/// For Hardware PWM, iCycle is fixed to 20000
/// ------------------------------------------------------------------
int		SIO_WriteMotor(int iRange, int iSpeed)
{
	static	char	sLog[256];
	static	int		bBoot = TRUE;

	if ( iRange < 1 )		iRange = 1;

	int 	iCycle	= 0;		// 1 * 16000;
	int		iOn;

	if ( bBoot == TRUE ) {
//		HAL_InitPWM(CAR_MOTOR_PM, PWM_MOTOR_CENTER);
		bBoot = FALSE;
	}

	iOn = iSpeed;
	if ( iOn >  300 ) 		iOn =  300;
	if ( iOn < -300 )		iOn = -300;

	iOn = (iOn > 0) ? PWM_MOTOR_CENTER + iOn : PWM_MOTOR_CENTER + iOn;

	if ( iRange == 1 )		iOn = 0;

	if ( pMM->hPWM == 0 ) {
		HAL_WritePWM(CAR_MOTOR_PM, iCycle, iOn);
	}
	else {
		char	sFIFO[256];

		sprintf(sFIFO, "M %d\n", iOn);
		HAL_WriteFIFO(pMM->hPWM, sFIFO, strlen(sFIFO));
	}

	sprintf(sLog, "[%6d:%6d]", iCycle, iOn);

	m_pReadLog = sLog;

	return (iOn);
}


/// ------------------------------------------------------------------
/// Interface Function
/// For Hardware PWM, iCycle is fixed to 20000
/// ------------------------------------------------------------------
int		SIO_WriteServo(int iRange, int iAngle)
{
	static	char	sLog[256];
	static	int		bBoot = TRUE;

	if ( iRange < 1 )		iRange = 1;

	int		iCenter	= 95;
	int		iTrim	= 0;
	int		iSpan	= 25 + 25;
	int		iMove	= (iSpan * iAngle / iRange) - iSpan / 2;

	int 	iCycle	= 0;		// 10 * 1000;
	int		iOn		= (iCenter + iTrim) + iMove;

	if ( bBoot == TRUE ) {
//		HAL_InitPWM(CAR_MOTOR_SM, PWM_SERVO_CENTER);
		bBoot = FALSE;
	}

	if ( iRange == 1 )		iOn = 0;

	if ( pMM->hPWM == 0 ) {
		HAL_WritePWM(CAR_MOTOR_SM, iCycle, iOn);
	}
	else {
		char	sFIFO[256];

		sprintf(sFIFO, "S %d\n", iOn);
		HAL_WriteFIFO(pMM->hPWM, sFIFO, strlen(sFIFO));
	}

	sprintf(sLog, "[%6d:%6d]", iCycle, iOn);

	m_pReadLog = sLog;

	return (iOn);
}


/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int		SIO_ReadIR()
{
	static	char	sLog[256];

	int		i;
	int		j;

	pMM->iIR[0] = HAL_ReadGPIO(CAR_IR0_SENSE);
	pMM->iIR[1] = HAL_ReadGPIO(CAR_IR1_SENSE);
	pMM->iIR[2] = HAL_ReadGPIO(CAR_IR2_SENSE);
	pMM->iIR[3] = HAL_ReadGPIO(CAR_IR3_SENSE);

	j = 0;
	sLog[0] = '[';

	for ( int i = 0 ; i < MAX_IR ; i++ ) {
		sLog[++j] = (pMM->iIR[i] == 1) ? 'O' : '.';
	}

	sLog[++j] = ']';
	sLog[++j] = 0;

	m_pReadLog = sLog;

	i  = 0;
	i += (pMM->iIR[0] * 1);
	i += (pMM->iIR[1] * 2);
	i += (pMM->iIR[2] * 4);
	i += (pMM->iIR[3] * 8);

	return (i);
}


#define MAX_US_TRIGTIME		10000
#define MAX_US_ECHOWAIT		100000
#define CAR_FF_OFFSET		60
#define CAR_RR_OFFSET		50
/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int		SIO_ReadDistUS(int bFront)
{
	static	char	sLog[256];

	int		iTrigPort	= (bFront) ? CAR_US_TRIG    : CAR_US_TRIG;
#ifdef USE_LIDAR
	int		iEchoPort	= CAR_US_RR_ECHO;
#else
	int		iEchoPort	= (bFront) ? CAR_US_FF_ECHO : CAR_US_RR_ECHO;
#endif
	int		iOffset		= (bFront) ? CAR_FF_OFFSET  : CAR_RR_OFFSET;

	int		iEchoRecv;
	int		iWait;
	int		iDistance	= 9999;

	HAL_GetTickCount(TRUE);

	HAL_WriteGPIO(iTrigPort, TRUE);
	HAL_WaitTickCount(MAX_US_TRIGTIME);

	HAL_WriteGPIO(iTrigPort, FALSE);
	HAL_GetTickCount(TRUE);

	while (TRUE) {
		iEchoRecv = HAL_ReadGPIO(iEchoPort);

		iWait = HAL_GetTickCount(FALSE);

		if ( (iEchoRecv == 0) && (iWait > MAX_US_ECHOWAIT) )	goto ERROR;
		if ( (iEchoRecv == 1) )									break;
	}

	HAL_GetTickCount(TRUE);

	while (TRUE) {
		iEchoRecv = HAL_ReadGPIO(iEchoPort);

		iWait = HAL_GetTickCount(FALSE);

		if ( (iEchoRecv == 1) && (iWait > MAX_US_ECHOWAIT) )	goto ERROR;
		if ( (iEchoRecv == 0) )									break;
	}

	if ( iEchoRecv == 0 ) {
		iDistance = (iWait / 2) * 340 / 1000;
		iDistance -= iOffset;
	}

	if (bFront == TRUE)		pMM->iUS_FF = iDistance;
	if (bFront == FALSE)	pMM->iUS_RR = iDistance;

ERROR:
	sprintf(sLog, "[%4d %5d]", iWait, iDistance);

	m_pReadLog = sLog;

	return (iDistance);
}


#define ZYRO_REG_WHOAMI	0x75
#define ZYRO_REG_PWR_M1	0x6b
#define ZYRO_REG_DATA	0x3b
#define xZYRO_REGADDR	0x13
#define ZYRO_REGLEN		14
/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
char*	SIO_ReadGyro()
{
	static	char	sLog[256];
	static	int		bTest = FALSE;
	static	int		bInit = TRUE;

	char	sREG[1];
	char	sZYRO[ZYRO_REGLEN];
	int		iRet;

	int		i;
	int		j;

	if ( bInit == TRUE ) {
		sREG[0]  = ZYRO_REG_PWR_M1;
		sZYRO[0] = 0;
		HAL_WriteI2C(sREG, 1, sZYRO, 1);	// Wake-Up MPU-6050

		sREG[0] = ZYRO_REG_WHOAMI;
		iRet =  HAL_ReadI2C(sREG, 1, sZYRO, 1);
	//	printf("MPU-6000 WHOAMI [%02x]\n", sZYRO[0]);

		bInit = FALSE;
	}

	sREG[0] = (bTest == TRUE) ? ZYRO_REG_WHOAMI : ZYRO_REG_DATA;
	iRet =  HAL_ReadI2C(sREG, 1, sZYRO, ZYRO_REGLEN);

	if ( bTest == FALSE ) {
		short*	pi = (short*)sZYRO;

		pMM->iAccel[0]  = pi[0];
		pMM->iAccel[1]  = pi[1];
		pMM->iAccel[2]  = pi[2];
		pMM->iRotate[1] = pi[3];
		pMM->iRotate[2] = pi[4];
		pMM->iRotate[3] = pi[5];
	}

	j = 0;
	sprintf(sLog, "[%02x]: ", sREG[0]);

	for ( int i = 0 ; i < ZYRO_REGLEN ; i++ ) {
		sprintf(sLog + strlen(sLog), "%02x ", sZYRO[i]);
	}

	j = strlen(sLog) - 2;

	sLog[++j] = ']';
	sLog[++j] = 0;

	m_pReadLog = sLog;

	return (sLog);
}


/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
char*	SIO_ReadGyroAccel(int* riX, int* riY, int* riZ)
{
	SIO_ReadGyro();

	*riX = pMM->iAccel[0];
	*riY = pMM->iAccel[1];
	*riZ = pMM->iAccel[2];

	return (m_pReadLog);
}


/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
char*	SIO_ReadGyroRotate(int* riX, int* riY, int* riZ)
{
	SIO_ReadGyro();

	*riX = pMM->iRotate[0];
	*riY = pMM->iRotate[1];
	*riZ = pMM->iRotate[2];

	return (m_pReadLog);
}


/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
char*	SIO_ReadGPS()
{
	static	char	sLog[256];

	HAL_SelectCOM(COM_GPS);

	sLog[0] = 0;

	if ( SIO_ReadCOM() == TRUE ) {
		sprintf(sLog, "[%s]", pMM->sCOM);
	}

	m_pReadLog = sLog;

	return (sLog);
}


/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int     SIO_ActivateLidar(int bOn)
{
// 	HAL_WriteGPIO(CAR_LIDAR_POWER, (bOn % 2));

	return (TRUE);
}


/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
char*	SIO_ReadLidar()
{
	static	char	sLog[10000];
	char*	pLog;

	char	cChar;
	int 	iRead 	= 0;

	sLog[0] = 0;

	HAL_SelectCOM(COM_LIDAR);

	pMM->iCOM = 0;

	for ( int i = 0 ; i < 3000 ; i++ ) {
		iRead = HAL_ReadCOM(0, &cChar, 1);

		if ( iRead == 0 )	break;

		// Load char in buffer
		pMM->sCOM[pMM->iCOM++] = cChar;
		pMM->sCOM[pMM->iCOM]   = 0;
	}

	pLog = sLog;
	sprintf(pLog, "[");
	pLog++;

	for ( int i = 0 ; i < pMM->iCOM ; i++ ) {
		if ( (pMM->sCOM[i] == 0xaa) && (pMM->sCOM[i+1] == 0x55) ) {
			sprintf(pLog, "] \n[");
			if (i > 0)		pLog += 4;
		}
		sprintf(pLog, "%02x ", pMM->sCOM[i]);
		pLog += 3;
	}
	sprintf(pLog, "]\n");
	pLog += 2;

	m_pReadLog = sLog;

	return (sLog);
}


/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int		SIO_ReadCOM()
{
	int 	iLen = 0;
	char	cChar;

	pMM->iCOM = 0;

	for ( ; ; ) {
	///	iLen = HAL_ReadCOM(0, pMM->sCOM, pMM->iCOM);
	///	if ( (iLen == 0) )						return (FALSE);

		iLen = HAL_ReadCOM(0, &cChar, 1);

		if ( (iLen == 0) && (cChar == '\n') )	continue;
		if ( (iLen == 0) && (cChar == '\r') )	continue;

		if ( (iLen >  0) && (cChar == '\n') )	return (TRUE);
		if ( (iLen >  0) && (cChar == '\r') )	return (TRUE);

		// Load char in buffer
		pMM->sCOM[pMM->iCOM++] = cChar;
		pMM->sCOM[pMM->iCOM]   = 0;
	}

	return (TRUE);
}


/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int		SIO_WriteCOM()
{
	HAL_WriteCOM(0, pMM->sCOM, pMM->iCOM);

	return (TRUE);
}
