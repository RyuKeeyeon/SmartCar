/*********************************************************************
    pwmd.c (Rev 0.92)

    SmartCar Main PWM Daemon

    Copyright(C)

    Programmed by  Stellar Respree
*********************************************************************/

#include <global.h>
#include "local.h"
#include "sw_type.h"
#include "hal_type.h"
#include "hal.h"


// -------------------------------------------------------------------
//	Type Definition
// -------------------------------------------------------------------
#define PWMD_VERSION	"V0.98"
#define USLEEP_1MS		(1 * 1000)
#define MOTOR_CYCLE		59000
#define SERVO_CYCLE		20000

#define MM_STOP_TIME	0


// -------------------------------------------------------------------
//	Global Variable
// -------------------------------------------------------------------
static	SWBOX	xMM;
static	SWBOX*	pMM = &xMM;


// -------------------------------------------------------------------
//	Main Function
// -------------------------------------------------------------------
int		main(int argc, char* argv[])
{
	FILE*	f = fopen("/tmp/pwmd.log", "w");
	int		iLog = 0;

	int		hPWM;
	char	sBuffer[1024];
	int		iWait = 0;
	int		iBuffer;

	printf("PWMD " PWMD_VERSION " Initializing....\n");
	fprintf(f, "PWMD " PWMD_VERSION " Initializing....\n");
	fflush(f);

///	pMM = SIO_Init();

	memset(pMM, 0, sizeof(SWBOX));
	HAL_Init(TRUE);
	hPWM = HAL_OpenFIFO(FIFO_PWM, FIFO_MODE);

	if (FALSE)
	{	// Di-Ro-Ri
		usleep(USLEEP_1MS * 500);
		int		iDDRO;

		/**/
		iDDRO = MM_CENTER_TIME + MM_SPEED_RANGE;
		printf("Init PWM %d\n", iDDRO);
		HAL_WritePWM(CAR_MOTOR_PM, 20000, iDDRO);
		usleep(USLEEP_1MS * 1000);

		iDDRO = MM_CENTER_TIME;
		printf("Init PWM %d\n", iDDRO);
		HAL_WritePWM(CAR_MOTOR_PM, 20000, iDDRO);
		usleep(USLEEP_1MS * 1000);
		/**/
		
		HAL_WritePWM(CAR_MOTOR_PM, 20000, MM_CENTER_TIME);
	}

	while (TRUE) {
		iWait = 0;

		while (TRUE) {
			iBuffer = HAL_ReadFIFO(hPWM, sBuffer, 1024);
			if ( iBuffer > 0 )	break;

			usleep(USLEEP_1MS);

			iWait++;
			if ( iWait == 3000 ) {
				printf("PWM AutoSleep with %d\n", MM_STOP_TIME);
				HAL_WritePWM(CAR_MOTOR_PM, MOTOR_CYCLE, MM_STOP_TIME);

				HAL_WriteGPIO(PIN_40, 0);

		///		printf("PWM AutoSleep with %d\n", 0);
		///		HAL_WritePWM(CAR_MOTOR_PM, 20000, 0);
				HAL_WritePWM(CAR_MOTOR_SM, 20000, 0);
			}
		}

		if ( iBuffer > 0 ) {
			char* 	p1;
			char* 	p2;
			char*	p;
			char* 	v;
			int		i;

			p1 = strtok(sBuffer, "\n\r");
			p2 = strtok(NULL, "\n\r");
		printf("FIFO out [%s %s] : ", p1, p2);
		if (++iLog < 100) {
			fprintf(f, "FIFO out [%s %s] : ", p1, p2);
			fflush(f);
		}

			if ( !strcmp(p1, "STOP") ) {
				break;
			}

			// P1 Processing
			if ( p1 != NULL ) {
				p = strtok(p1, " ,\t\n\r");
				v = strtok(NULL,    " ,\t\n\r");

				if ( v != NULL )
				i = atoi(v);

				if ( p[0] == 'C' )		HAL_WritePWM(CAR_MOTOR_PM, 20000, 0);
				if ( p[0] == 'C' )		HAL_WritePWM(CAR_MOTOR_SM, 20000, 0);

				if ( p[0] == 'M' )		HAL_WritePWM(CAR_MOTOR_PM, 20000, i);
				if ( p[0] == 'S' )		HAL_WritePWM(CAR_MOTOR_SM, 20000, i);
			}

			// P2 Processing
			if ( p2 != NULL ) {
				p = strtok(p2, " ,\t\n\r");
				v = strtok(NULL,    " ,\t\n\r");

				if ( v != NULL )
				i = atoi(v);

				if ( p[0] == 'C' )		HAL_WritePWM(CAR_MOTOR_PM, 20000, 0);
				if ( p[0] == 'C' )		HAL_WritePWM(CAR_MOTOR_SM, 20000, 0);

				if ( p[0] == 'M' ) 		HAL_WritePWM(CAR_MOTOR_PM, 20000, i);
				if ( p[0] == 'S' ) 		HAL_WritePWM(CAR_MOTOR_SM, 20000, i);
			}

		printf("\n");
		}
	}

	return(TRUE);
}
