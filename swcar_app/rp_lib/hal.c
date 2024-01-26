/*********************************************************************
    hal.c (Rev 1.00)

    HAL Processing Module  (RaspberryPi GPIO/I2C/SPI)

    Copyright(C) Memorylab Ltd.

    Programmed by  Stellar Respree
*********************************************************************/

#include <global.h>
#include "local.h"
#ifdef RASPBIAN
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <asm/ioctl.h>
#include <sys/time.h>

#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <wiringSerial.h>
#include <termios.h>
#else
#endif
#include "hal_type.h"
#include "hal.h"


/// ------------------------------------------------------------------
/// Static Definition
/// ------------------------------------------------------------------
#define GPIO_MAX		40
#define GPrW_WRITE		1
#define GPrW_READ		0

#define I2C_DEVICE		"/dev/i2c-1"

#define COM_DEVICE1		"/dev/ttyAMA1"
#define COM_DEVICE2		"/dev/ttyAMA2"
#define COM_DEVICE3		"/dev/ttyAMA3"
#define COM_DEVICE4		"/dev/ttyAMA4"
#define COM_SPEED		9600


/// ------------------------------------------------------------------
/// Type Definition
/// ------------------------------------------------------------------
typedef struct _PWM {
	int		iCycle;
	int		iOn;
} PWM;


/// ------------------------------------------------------------------
/// Static Variable  (BCM => wPi Port Mapping)
/// ------------------------------------------------------------------
static	int		WIRING_PI_PORT[] = {
	30, 31,  8,  9,  7,
	21, 22, 11, 10, 13,
	12, 14, 26, 23, 15,
	16, 27,  0,  1, 24,
	28, 29,  3,  4,  5,
	 6, 25,  2, -1, -1};


/// ------------------------------------------------------------------
/// Static Variable
/// ------------------------------------------------------------------
static	int		m_hGPrW[1 + GPIO_MAX];
static	int		m_hI2C;
static	int		m_iIID;
static	int		m_iSPI;

static	int		m_iComID;		// Default COMM Port
static	int		m_hCOM1;
static	int		m_hCOM2;
static	int		m_hCOM3;
static	int		m_hCOM4;

static	char	m_sDEBUG_PIO[100];
static	char	m_sDEBUG_COM[100];
static	int		m_iDEBUG_COM;


/// ------------------------------------------------------------------
/// Private Function
/// ------------------------------------------------------------------
void	pwm_Init()
{
	pinMode(WIRING_PI_PORT[PIN_32], PWM_OUTPUT);
	pinMode(WIRING_PI_PORT[PIN_33], PWM_OUTPUT);

	pwmSetMode(PWM_MODE_MS);
///	pwmSetClock(384);	// 19.2 * 10^6 / 384 * 1000 = 50Hz = 20ms
///	pwmSetClock(19200 *  X / 1000);	// 19.2e6 / 19.2 / X / 1000 = X ms
	pwmSetClock(19200 * 16 / 1000);	// 16 ms
	pwmSetRange(1000);	// 1 is 20us

	while (0) {
		int		iPort = PIN_32;
		pwmWrite(WIRING_PI_PORT[iPort], 80);
		delay(3000);
	}
}


/// ------------------------------------------------------------------
/// Private Function
/// ------------------------------------------------------------------
void	pwm_Free()
{
}


/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int		HAL_Init(int bUsePWM)
{
#ifdef RASPBIAN
	char 	cData[256];

	memset( m_hGPrW, 0xff, sizeof(m_hGPrW) );

	if ( wiringPiSetup() == -1 )						return (FALSE);
///	if ( wiringPiSPISetup(SPI_CH, SPI_SPEED) == -1 )	return (FALSE);
#else
	memset(m_sDEBUG_PIO, 0, 100);
#endif

	if ( bUsePWM == TRUE ) {
		pwm_Init();
	}

	return (TRUE);
}


/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int		HAL_Free()
{
	pwm_Free();

	return (TRUE);
}


// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int		HAL_WriteGPIO(int iPort, int bOn)
{
#ifdef RASPBIAN
	if ( m_hGPrW[iPort] != GPrW_WRITE ) {
		pinMode( WIRING_PI_PORT[iPort], OUTPUT );
		m_hGPrW[iPort] = GPrW_WRITE;
	}

	digitalWrite(WIRING_PI_PORT[iPort], bOn);
#else
	m_sDEBUG_PIO[iPort] = bOn;
#endif

	return (TRUE);
}


/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int		HAL_ReadGPIO(int iPort)
{
#ifdef RASPBIAN
	if ( m_hGPrW[iPort] != GPrW_READ ) {
		pinMode( WIRING_PI_PORT[iPort], INPUT );
		m_hGPrW[iPort] = GPrW_READ;
	}

	int		iVal = digitalRead( WIRING_PI_PORT[iPort] );
#else
	int		iVal = m_sDEBUG_PIO[iPort];
#endif

        return (iVal);
}


/// ------------------------------------------------------------------
/// Interface Function
/// For Hardware PWM, iOn resolution is 20ms
/// ------------------------------------------------------------------
int		HAL_WritePWM(int iPort, int iCycle, int iOn)
{
///	iOn = iOn / 20;

	pwmWrite(WIRING_PI_PORT[iPort], iOn);

	return (TRUE);
}


/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int     HAL_OpenI2C(char* pDevName, int iID)
{
	if ( pDevName == NULL )		pDevName = I2C_DEVICE;

	m_hI2C = open(pDevName, O_RDWR);

	if ( m_hI2C < 0 )		return (FALSE);

	m_iIID = iID;

	return (TRUE);
}


/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int     HAL_CloseI2C()
{
	close(m_hI2C);

	m_hI2C = 0;

	return (TRUE);
}


/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int     HAL_SelectI2C(int iID)
{
	m_iIID = iID;

	return (TRUE);
}


/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int		HAL_WriteI2C(char* pReg, int iReg, char* pData, int iData)
{
#ifdef RASPBIAN
	uint8_t 	bin[256];

	if ( iReg  > 0 )	memcpy(bin,        pReg,  iReg);
	if ( iData > 0 )	memcpy(bin + iReg, pData, iData);

	struct i2c_rdwr_ioctl_data	tran;
	struct i2c_msg				msgs[2];

	msgs[0].addr	= m_iIID;
	msgs[0].flags	= 0;
	msgs[0].len		= iReg + iData;
	msgs[0].buf		= bin;

	tran.msgs		= msgs;
	tran.nmsgs		= 1;

	return ioctl( m_hI2C, I2C_RDWR, &tran );

#else
	return (TRUE);
#endif
}


/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int		HAL_ReadI2C(char* pReg, int iReg, char* pData, int iData)
{
#ifdef RASPBIAN
	uint8_t 	bin[256];

	if ( iReg  > 0 )	memcpy(bin, pReg, iReg);

	struct i2c_rdwr_ioctl_data	tran;
	struct i2c_msg				msgs[2];

	if ( iReg > 0 ) {
		msgs[0].addr	= m_iIID;
		msgs[0].flags	= 0;
		msgs[0].len		= iReg;
		msgs[0].buf		= pReg;

		msgs[1].addr	= m_iIID;
		msgs[1].flags	= I2C_M_RD;
		msgs[1].len		= iData;
		msgs[1].buf		= (UCHAR*)pData;

		tran.msgs		= msgs;
		tran.nmsgs		= 2;
	}
	else {
		msgs[0].addr	= m_iIID;
		msgs[0].flags	= I2C_M_RD;
		msgs[0].len		= iData;
		msgs[0].buf		= (UCHAR*)pData;

		tran.msgs		= msgs;
		tran.nmsgs		= 1;
	}

	return ioctl( m_hI2C, I2C_RDWR, &tran );

#else
	return (TRUE);
#endif
}


/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int     HAL_OpenSPI(int iCh, int iSpeed)
{
#ifdef RASPBIAN
	if ( wiringPiSPISetup(iCh, iSpeed) == -1 )	return (FALSE);
#else
	// TODO
#endif
	m_iSPI = iCh;

	return (TRUE);
}


/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int     HAL_CloseSPI()
{
	m_iSPI = 0;

	return (TRUE);
}


/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int		HAL_WriteSPI(char* pBuffer, int iLength)
{
#ifdef RASPBIAN
	wiringPiSPIDataRW(m_iSPI, (UCHAR*)pBuffer, iLength);
#else
	// TODO
#endif

	return (TRUE);
}


/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int		HAL_ReadSPI(char* pBuffer, int iLength)
{
#ifdef RASPBIAN
	wiringPiSPIDataRW(m_iSPI, (UCHAR*)pBuffer, iLength);
#else
	// TODO
#endif

	return (TRUE);
}


/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int		HAL_OpenCOM(int iComID, int iSpeed, int iOption)
{
	int		h;

	if ( iComID == 0 )		iComID	= m_iComID;
	if ( iSpeed == 0 )		iSpeed	= COM_SPEED;

#ifdef RASPBIAN
	if ( iComID == 1 )		h = m_hCOM1 = serialOpen(COM_DEVICE1, iSpeed);
	if ( iComID == 2 )		h = m_hCOM2 = serialOpen(COM_DEVICE2, iSpeed);
	if ( iComID == 3 )		h = m_hCOM3 = serialOpen(COM_DEVICE3, iSpeed);
	if ( iComID == 4 )		h = m_hCOM4 = serialOpen(COM_DEVICE4, iSpeed);

	if ( h == -1 )			return (FALSE);

	m_iComID = iComID;
#else
	memset(m_sDEBUG_COM, 0, 100);
	m_iDEBUG_COM = 0;
#endif

#ifdef RASPBIAN
	if ( iOption == 10 ) {
		struct  termios xOpt;
		tcgetattr(h, &xOpt);
		xOpt.c_cflag |= PARENB;
		tcsetattr(h, TCSANOW, &xOpt);
	}
#endif

	return (iComID);
}


/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int		HAL_CloseCOM(int iComID)
{
	int		h;

	if ( iComID == 0 )		iComID	= m_iComID;
	if ( iComID == 1 )		h = m_hCOM1;
	if ( iComID == 2 )		h = m_hCOM2;
	if ( iComID == 3 )		h = m_hCOM3;
	if ( iComID == 4 )		h = m_hCOM4;

#ifdef RASPBIAN
	serialClose(h);
#else
#endif

	return (TRUE);
}


/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int     HAL_SelectCOM(int iComID)
{
	m_iComID = iComID;

	return (iComID);
}


/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int		HAL_WriteCOM(int iComID, char* pBuffer, int iLength)
{
	int		h;

	if ( iComID == 0 )		iComID	= m_iComID;
	if ( iComID == 1 )		h = m_hCOM1;
	if ( iComID == 2 )		h = m_hCOM2;
	if ( iComID == 3 )		h = m_hCOM3;
	if ( iComID == 4 )		h = m_hCOM4;

	pBuffer[iLength] = 0;

#ifdef RASPBIAN
	for ( int i = 0 ; i < iLength ; i++ ) {
		serialPutchar(h, pBuffer[i]);
	}
	serialFlush(h);
#else
	strcpy(m_sDEBUG_COM, pBuffer);
	m_iDEBUG_COM = 0;
#endif

	return (TRUE);
}


#define XXX_COM_WAITTIME		3000
#define COM_WAITTIME		100
/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int		HAL_ReadCOM(int iComID, char* pBuffer, int iLength)
{
	int		h;
	int		iRead;

	if ( iComID == 0 )		iComID	= m_iComID;
	if ( iComID == 1 )		h = m_hCOM1;
	if ( iComID == 2 )		h = m_hCOM2;
	if ( iComID == 3 )		h = m_hCOM3;
	if ( iComID == 4 )		h = m_hCOM4;

	pBuffer[0] = 0;

#ifdef RASPBIAN
	int		iUsec = 0;

	for ( iRead = 0 ; iRead < iLength ; iRead++ ) {
		while (TRUE) {
			if ( serialDataAvail(h) > 0 )			break;

		//	if ( (++iUsec) == COM_WAITTIME ) 		return (iRead);
			if ( (++iUsec) == COM_WAITTIME ) 		return (FALSE);

			usleep(1000);
		}

		pBuffer[iRead    ] = serialGetchar(h);
		pBuffer[iRead + 1] = 0;
	}

	return (iRead);
#else
	return ( m_sDEBUG_COM[m_iDEBUG_COM++] );
#endif
}


/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int		HAL_ReadOneWire(int iPort)
{
	return (TRUE);
}


/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int		HAL_OpenFIFO(char* pName, int iMode)
{
	int		hf;

	remove(pName);

//	if ( mkfifo(pName, iMode) < 0 ) {
	if ( mkfifo(pName, 0777) < 0 ) {
	//	AddLog((char*)"HAL", (char*)"FIFO Creation Fail");
	///	return (FALSE);
	}

	hf = open(pName, O_RDWR | O_NONBLOCK);
	if ( hf < 0 ) {
	//	AddLog((char*)"HAL", (char*)"FIFO Open Fail");
		return (FALSE);
	}

	chmod(pName, 0777);

	return (hf);
}


/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int		HAL_CloseFIFO(int hHandle)
{
	close(hHandle);

	return (TRUE);
}


/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int		HAL_WriteFIFO(int hHandle, char* pBuffer, int iLength)
{
	int		iLen;

	iLen = write(hHandle, pBuffer, iLength);

	if ( iLen < 0 )		return (FALSE);

	return (iLen);
}


/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int		HAL_ReadFIFO(int hHandle, char* pBuffer, int iLength)
{
	int		iLen;

	iLen = read(hHandle, pBuffer, iLength);

	if ( iLen > 0 )		pBuffer[iLen] = 0;

	return (iLen);
}


#define SECOND_KST			(9 * 3600)
#define SECOND_PER_DAY		86400
/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int		HAL_GetLocalTime()
{
	int		iTime;

	iTime = (time(NULL) + SECOND_KST) % SECOND_PER_DAY;

	return (iTime);
}


static	int		m_iInitTime;
/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int		HAL_GetTickCount(int bClear)
{
	struct	timespec	xTS;
	int		iTickTime;

	clock_gettime(CLOCK_MONOTONIC, &xTS);
//	printf("%d %d\n", xTS.tv_sec, xTS.tv_nsec);

	iTickTime = xTS.tv_sec % 1000 + xTS.tv_nsec / 1000;

	if ( bClear == TRUE )			m_iInitTime = iTickTime;

	if ( iTickTime < m_iInitTime )	m_iInitTime -= (1000 * 1000 * 1000);

	return (iTickTime - m_iInitTime);
}


/// ------------------------------------------------------------------
/// Interface Function
/// ------------------------------------------------------------------
int		HAL_WaitTickCount(int iUS)
{
	struct	timespec	xTS;
	int		iTickTime;

	while (TRUE) {
		clock_gettime(CLOCK_MONOTONIC, &xTS);
	//	printf("%d %d\n", xTS.tv_sec, xTS.tv_nsec);

		iTickTime = xTS.tv_sec % 1000 + xTS.tv_nsec / 1000;
		if ( iTickTime < m_iInitTime )	m_iInitTime -= (1000 * 1000 * 1000);

		iTickTime = iTickTime - m_iInitTime;

		if ( iTickTime >= iUS )		break;
	}

	return (iTickTime);
}
