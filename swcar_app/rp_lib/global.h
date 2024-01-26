/*********************************************************************
    global.h  (Rev 2.5)

    Global Include file for standard library and stlib

    Copyright(C) 2001-2022  Stellar Respree

    Programmed by Stellar Respree
*********************************************************************/

#ifndef __GLOBAL_H__
#define __GLOBAL_H__


//--------------------------------------------------------------------
//	Standard Include Section
//--------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <signal.h>
#include <errno.h>

#ifdef WIN32
#include <windows.h>
#include <winsock.h>
#else
#include <strings.h>
#include <pthread.h>
#include <unistd.h>
#include <pwd.h>
#include <syslog.h>
#include <arpa/inet.h>
#endif


//--------------------------------------------------------------------
//	Suggested Basic Type Definition
//--------------------------------------------------------------------
#ifndef TRUE
#define TRUE                    1
#endif
#ifndef FALSE
#define FALSE                   0
#endif
#ifndef STRING
#define STRING					255
#endif

#ifndef UCHAR
#define UCHAR                   unsigned char
#endif
#ifndef UINT
#define UINT                    unsigned int
#endif
#ifndef ULONG
#define ULONG                   unsigned long
#endif
#ifndef WCHAR
#define WCHAR					wchar_t
#endif


/*--------------------------------------------------------------------
    Common Used Structures Definition
--------------------------------------------------------------------*/
#ifdef WIN32
#else
#define DEFINE_HANDLE
#endif


#ifdef DEFINE_HANDLE
typedef	UINT					HANDLE;
#endif

#ifdef DEFINE_LINK
typedef void*					LINK;
#endif

#ifdef DEFINE_BIN
typedef struct _BIN {
	int		size;
	char*	data;
} 								BIN;
#endif


//--------------------------------------------------------------------
//	Global Variable Support
//--------------------------------------------------------------------
#ifdef MAIN
#define EXTERN
#define	DEFAULT(a)				= a
#else
#define EXTERN					extern
#define	DEFAULT(a)
#endif

#endif	// global.h
