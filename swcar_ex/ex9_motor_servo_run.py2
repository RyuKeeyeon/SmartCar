from ctypes import*
import os
import time

WiringPi = CDLL("/home/pi/WiringPi/wiringPi/libwiringPi.so.2.70", mode=RTLD_GLOBAL)
swcar = cdll.LoadLibrary('/home/pi/swcar_lib/librp_smartcar.so')

swcar.SIO_Init(0)

print('Press ctrl + c to terminate program')
print('Running Servo Motor.')

iAngle = 0
iInc = 4;
while(True):
    iAngle = iAngle + iInc
    if (iAngle >= 100) :
        iInc = -4;
    if (iAngle <= 0) :
        iInc = 4;
    
    swcar.SIO_WriteServo(100, iAngle)
    print("Angle is " , iAngle)
    time.sleep(0.10)
    


