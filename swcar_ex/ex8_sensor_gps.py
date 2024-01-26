from ctypes import*
import os
import time

WiringPi = CDLL("/home/pi/WiringPi/wiringPi/libwiringPi.so.2.70", mode=RTLD_GLOBAL)
swcar = cdll.LoadLibrary('/home/pi/swcar_lib/librp_smartcar.so')

swcar.SIO_Init(0)

print('Press ctrl + c to terminate program')

swcar.SIO_ReadGPS.restype = c_char_p
swcar.SIO_ReadGPS.argtypes = None


while(True):
    gps = swcar.SIO_ReadGPS()
    print("gps = ", gps)
    
    time.sleep(1)
    
    
