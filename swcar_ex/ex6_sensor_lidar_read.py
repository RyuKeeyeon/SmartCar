from ctypes import*
import os
import time

WiringPi = CDLL("/home/pi/WiringPi/wiringPi/libwiringPi.so.2.70", mode=RTLD_GLOBAL)
swcar = cdll.LoadLibrary('/home/pi/swcar_lib/librp_smartcar.so')

swcar.SIO_Init(0)

print('Press ctrl + c to terminate program')

swcar.SIO_ReadLidar.restype = c_char_p
swcar.SIO_ReadLidar.argtypes = None

swcar.SIO_ActivateLidar(1);
    
while(True):
    lidar = swcar.SIO_ReadLidar()
    print("lidar info = ", lidar)
    time.sleep(1)