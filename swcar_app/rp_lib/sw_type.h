/*********************************************************************
    sw_type.h (Rev 0.94)

    SwitchBox ADT

    Copyright(C) Memorylab Ltd.

    Programmed by  Stellar Respree
*********************************************************************/

// -------------------------------------------------------------------
//	Type Definition
// -------------------------------------------------------------------
#define FIFO_LEN		10240
#define FIFO_MODE		0666
#define FIFO_PWM		"/tmp/SWCAR_PWM"

#ifdef USE_LIDAR
#define MAX_SW			3
#else
#define MAX_SW			4
#endif
#define MAX_LED			2
#define MAX_IR			4
#define MAX_AXIS		3

#define MM_CENTER_TIME	1500
#define MM_MARGIN_TIME	0
#define MM_SPEED_RANGE	200


typedef struct SWBOX_ {
	HANDLE	hPWM;						// Used for PWM Pipe
	HANDLE	hCOM_GPS;					// Used for GPS
	HANDLE	hCOM_LDA;					// Used for Lidar

	char	sPIO[FIFO_LEN + 1];			// For PIO Log Message Only

	char	sI2C[FIFO_LEN + 1];
	char	sSPI[FIFO_LEN + 1];

	char	sCOM[FIFO_LEN + 1];
	int		iCOM;

	/// =============================

	int		iSW[MAX_SW];
	int		iLED[MAX_LED];

	int		iIR[MAX_IR];

	int		iUS_FF;
	int		iUS_RR;
	int		iLS;

	int		iAccel[MAX_AXIS];
	int		iRotate[MAX_AXIS];
}
SWBOX;
