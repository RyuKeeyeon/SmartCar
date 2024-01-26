#define LOCAL

#include <fcntl.h>

#include <ifaddrs.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/reboot.h>


#ifdef RASPBIAN
#define NIO_IF_NAME			"wlan0"
#define NIO_REBOOT_SH		"/home/pi/swbox/bb.sh"
#endif


#define	SWBOX_VERSION		"VERSION_0.93.0"
#define SWBOX_SERIAL		"1000_3000_1225"

#define USE_LIDAR           1
