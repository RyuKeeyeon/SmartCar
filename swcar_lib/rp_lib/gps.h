/*********************************************************************
	gps.h  (Rev 0.80)

	GPS Receiver Processing Module

	Copyright(C) 2022 by Memorylab Ltd.

	Programmed by Stellar Respree
*********************************************************************/

#ifdef __GPS_H__
#define __GPS_H__

#ifdef __cplusplus
extern "C" {
#endif


/*--------------------------------------------------------------------
	Function Prototype
--------------------------------------------------------------------*/
int		GPS_Init(int hUART);
int		GPS_Reset();
int		GPS_PushDataQueue(char* pRecv);


#ifdef __cplusplus
}
#endif

#endif
