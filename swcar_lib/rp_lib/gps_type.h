/*********************************************************************
    gps_type.h (Rev 0.92)

    GPS ADT

    Copyright(C) Memorylab Ltd.

    Programmed by  Stellar Respree
*********************************************************************/

// -------------------------------------------------------------------
//	Type Definition
// -------------------------------------------------------------------
typedef struct INFO_ {
	int	iGPS_X;
	int	iGPS_Y;
	int	iGPS_A;
	int	iGPS_Speed;
}
INFO;


typedef struct GPS_MM_ {
	HANDLE	hUART;

	INFO	N;	// New Info
	INFO	S;	// Stable Info
}
GPS_MM;

