/*********************************************************************
    boot_car.c (Rev 0.10)

    SmartCar Boot Program

    Copyright(C)

    Programmed by  Stellar Respree
*********************************************************************/

#include <global.h>
#include "local.h"
#include "sw_type.h"
#include "sio.h"
#include <wiringPi.h>


// -------------------------------------------------------------------
//	Type Definition
// -------------------------------------------------------------------


// -------------------------------------------------------------------
//	Global Variable
// -------------------------------------------------------------------


// -------------------------------------------------------------------
//	Global Variable
// -------------------------------------------------------------------
int		GetIP();


// -------------------------------------------------------------------
//	Main Function
// -------------------------------------------------------------------
int		main(int argc, char* argv[])
{
	int		hPWM;
	char	sBuffer[1024];
	int		iBuffer;

	int		iIP;

	printf("BootCar Initializing....\n");

	SIO_Init(FALSE);

	iIP = GetIP();
	SIO_WriteLED(iIP);

	printf("IP %d\n", iIP);

	/*
	{
		FILE*	f = fopen("my.log", "w");
		fprintf(f, "%d\n", iIP);
		fclose(f);
	}
	*/

	return(TRUE);
}


#define SEEK_STRING		"static ip_address="
// -------------------------------------------------------------------
//	Local Function
// -------------------------------------------------------------------
int		GetIP()
{
	char	sBuffer[1024];
	int		iBuffer;
	char*	p;
	int		iIP = 0;

///	FILE* 	f = fopen("./boot_ip.log", "r");
	FILE* 	f = fopen("/etc/dhcpcd.conf", "r");

	while ( !feof(f) ) {
		fgets(sBuffer, 1024, f);

		if ( strncmp(sBuffer, SEEK_STRING, strlen(SEEK_STRING)) ) continue;

		p = strtok(sBuffer, "=./\n");

		p = strtok(NULL, " ./\n");
		p = strtok(NULL, " ./\n");
		p = strtok(NULL, " ./\n");
		p = strtok(NULL, " ./\n");

		iIP = atoi(p);

		break;
	}

	fclose(f);

	return (iIP);
}

