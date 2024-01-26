/*********************************************************************
	gps.c  (Rev 0.80)

	GPS Receiver Processing Module

	Copyright(C) 2022 by Memorylab Ltd.

	Programmed by Stellar Respree
*********************************************************************/

#include <global.h>
#include "local.h"
#include "gps_type.h"
#include "gps.h"


/*--------------------------------------------------------------------
	Type Definition
--------------------------------------------------------------------*/
#define TOK_INT			1
#define TOK_FLT			2
#define TOK_STR			3


GPS_MM	m_xGPS;
GPS_MM*	m_pGPS = &m_xGPS;
/*--------------------------------------------------------------------
	Function Prototype
--------------------------------------------------------------------*/
int		GPS_Init(int hUART)
{
	m_pGPS->hUART = hUART;

	return (TRUE);
}


#define COLD_RESET		"$PSRF101,0,0,0,000,0,0,12,4*10\r\n"
#define WARM_RESET		"$PSRF101,0,0,0,000,0,0,12,4*16\r\n"
#define HOT_RESET		"$PSRF101,0,0,0,000,0,0,12,4*15\r\n"
/*--------------------------------------------------------------------
	Function Prototype
--------------------------------------------------------------------*/
int		GPS_Reset()
{
	printf("no Reset GPS\n");
	return (TRUE);

	// Write Data
	write(m_pGPS->hUART, WARM_RESET, strlen(WARM_RESET));

	return (TRUE);
}


int		gps_DecodeData(char* pPacket, int iPacket);
int		gps_GetToken(char* pPacket, const char* pSepa, int iType, int* iInt, int* iFrag, char** rpStr);
int		gps_UpdateSystemTime(int iData, int iTime, int iMilliSecond);
/*--------------------------------------------------------------------
	Function Prototype
--------------------------------------------------------------------*/
int		GPS_PushDataQueue(char* pRecv)
{
	static	char	sPacket[1024];		// Protocol Data
	static	int		iPacket 	= 0;
	static	int		bPending	= FALSE;
	int				iGPS;

	char*			pSeek;
	char*			pNext;

	if ( bPending == FALSE ) {
		pSeek = strchr(pRecv, '$');
		if ( pSeek == NULL )		return (TRUE);

		strcpy(sPacket, pSeek);
		bPending = TRUE;
	}
	else {							// Append Packet
		strcat(sPacket, pSeek);
	}

	pSeek = strchr(sPacket, '\n');
	if ( pSeek == NULL )			return (TRUE);

	pSeek[-1] = 0;
	/// ================================================
	iGPS = gps_DecodeData(sPacket, strlen(sPacket));	// Decode !!
	/// ================================================

	if ( pSeek[1] == 0 ) {
		bPending = FALSE;
		return (TRUE);
	}

	pSeek = strrchr(pSeek + 1, '$');								// Find last header
	if ( pSeek != NULL ) {											// (ignore middle packet)
		strcpy(sPacket, pSeek);
		bPending = TRUE;
	}

	return (TRUE);
}


#define COMMAND_GGA		('G' << 16) + ('G' << 8) + ('A')
#define COMMAND_RMC		('R' << 16) + ('M' << 8) + ('C')
#define COMMAND_VTG		('V' << 16) + ('T' << 8) + ('G')
/*--------------------------------------------------------------------
	Function Prototype
--------------------------------------------------------------------*/
int		gps_DecodeData(char* pPacket, int iPacket)
{
	int		iCommand;
	char*	pSeek;

	int		iDate;
	int		iTime;
	int		iMilliSecond;
	int		iX;
	int		iY;
	int		iA;
	int		iSpeed;
	int		i;
	int		j;

	static int	iTimeReset	= TRUE;

#ifdef DEBUG_PRINT_GPS
	printf("  [GPS:%s]\n", pPacket);
#endif

	if ( pPacket[0] != '$' )	return (FALSE);

	iCommand = (pPacket[3] << 16) + (pPacket[4] << 8) + pPacket[5];

	switch (iCommand) {
	case COMMAND_RMC:			// DateTime, Position, Speed
//		printf("  [GPS:%s]\n", pPacket);
		// $GPRMC,000419.041,A,0000.0000,N,00000.0000,E,,,291006,,*11
		if ( gps_GetToken(pPacket, ",", TOK_STR, NULL, NULL, &pSeek) == 0 )		return (FALSE);
		if ( gps_GetToken(NULL, ",", TOK_FLT, &iTime, &iMilliSecond, NULL) == 0) return (FALSE);
		if ( gps_GetToken(NULL, ",", TOK_STR, NULL, NULL, &pSeek) == 0 )			return (FALSE);
		if ( pSeek[0] != 'A' )													return (FALSE);

		if ( gps_GetToken(NULL, ",", TOK_FLT, &i, &j, &pSeek) == 0 )				return (FALSE);
		if ( gps_GetToken(NULL, ",", TOK_STR, NULL, NULL, &pSeek) == 0 )			return (FALSE);
		if ( pSeek[0] == 'E' )		iX = i * 10000 + j;
		if ( pSeek[0] == 'W' )		iX = i * 10000 + j;
		if ( pSeek[0] == 'N' )		iY = i * 10000 + j;
		if ( pSeek[0] == 'S' )		iY = i * 10000 + j;

		if ( gps_GetToken(NULL, ",", TOK_FLT, &i, &j, &pSeek) == 0 )				return (FALSE);
		if ( gps_GetToken(NULL, ",", TOK_STR, NULL, NULL, &pSeek) == 0 )			return (FALSE);
		if ( pSeek[0] == 'E' )		iX = i * 10000 + j;
		if ( pSeek[0] == 'W' )		iX = i * 10000 + j;
		if ( pSeek[0] == 'N' )		iY = i * 10000 + j;
		if ( pSeek[0] == 'S' )		iY = i * 10000 + j;

		if ( gps_GetToken(NULL, ",", TOK_FLT, &i, &j, &pSeek) == 0 )				return (FALSE);
		iSpeed = (i * 100 + j) * 10;			// milli-knot
		iSpeed = iSpeed * 1852 / 1000000;		// kms conversion  (km/s)

		if ( gps_GetToken(NULL, ",", TOK_FLT, &i, &j, &pSeek) == 0 )				return (FALSE);
		iA = i;									// ignore frac data

		if ( gps_GetToken(NULL, ",", TOK_INT, &i, NULL, NULL) == 0 )				return (FALSE);
		iDate = (i % 100) * 10000 + ((i / 100) % 100) * 100 + (i / 10000);

		// Final Data Collection
		m_pGPS->N.iGPS_X		= iX;
		m_pGPS->N.iGPS_Y		= iY;
		m_pGPS->N.iGPS_A		= iA;
		m_pGPS->N.iGPS_Speed	= iSpeed;

		// Time Adjustment
		if ( iTimeReset == TRUE ) {
			iTimeReset = FALSE;
			gps_UpdateSystemTime(iDate, iTime, iMilliSecond);
		}

		break;

	case COMMAND_GGA:
	case COMMAND_VTG:
	default:
		return (FALSE);
	}

	return (TRUE);
}


/*--------------------------------------------------------------------
	Function Prototype
--------------------------------------------------------------------*/
int		gps_GetToken(char* pPacket, const char* pSepa, int iType, int* iInt, int* iFrag, char** rpStr)
{
	static char*	pLast = NULL;
	char*			pHead;
	char*			pSeek;

	if ( pPacket != NULL )		pLast = pPacket;
	if ( pLast == NULL )		return (FALSE);

	pHead = pLast;
	pSeek = strchr(pHead, pSepa[0]);
	if ( pSeek == NULL )		pSeek = pHead + strlen(pHead);

	pSeek[0] = 0;
	pLast = pSeek + 1;

	switch (iType) {
	case TOK_INT:
		*iInt = 0;

		if ( pHead != pSeek ) {
			*iInt	= atoi(pHead);
		}
		break;
	case TOK_FLT:
		*iInt = 0;
		*iFrag = 0;

		if ( pHead != pSeek ) {
			pSeek = strchr(pHead, '.');
			if ( pSeek == NULL )	return (FALSE);

			pSeek[0] = 0;
			*iInt	= atoi(pHead);
			*iFrag	= atoi(pSeek + 1);
		}
		break;
	case TOK_STR:
		*rpStr = NULL;

		if ( pHead != pSeek ) {
			*rpStr	= pHead;
		}
		break;
	default:
		return (FALSE);
	}

	return (TRUE);
}


/*--------------------------------------------------------------------
	Function Prototype
--------------------------------------------------------------------*/
int		gps_UpdateSystemTime(int iData, int iTime, int iMilliSecond)
{
	return (TRUE);
}

