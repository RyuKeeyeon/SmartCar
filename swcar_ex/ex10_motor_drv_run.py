from ctypes import*
import os
import time

WiringPi = CDLL("/home/pi/WiringPi/wiringPi/libwiringPi.so.2.70", mode=RTLD_GLOBAL)
swcar = cdll.LoadLibrary('/home/pi/swcar_lib/librp_smartcar.so')

swcar.SIO_Init(0)

print('Press ctrl + c to terminate program')
print('Running Driving Motor.')

swcar.SIO_MaxMotorSpeed(100)
swcar.SIO_BrakeBLDC(1)

for i in range(1,30):
    speed = i * 5;
    if ( speed >= 100 ):
        speed = 100;
    swcar.SIO_WriteBLDC(speed)
    print("Forward Speed is %d" %speed)
    time.sleep(1)

for i in range(30,1,-1):
    speed = i * 5;
    if ( speed >= 100 ):
        speed = 100;
    swcar.SIO_WriteBLDC(speed)
    print("Forward Speed is %d" %speed)
    time.sleep(1)

for i in range(1,30):
    speed = i * 5;
    if ( speed >= 100 ):
        speed = 100;
    swcar.SIO_WriteBLDC(-speed)
    print("Forward Speed is %d" %speed)
    time.sleep(1)

for i in range(30,1,-1):
    speed = i * 5;
    if ( speed >= 100 ):
        speed = 100;
    swcar.SIO_WriteBLDC(-speed)
    print("Forward Speed is %d" %speed)
    time.sleep(1)

swcar.SIO_BrakeBLDC(1)

print('Stop Driving Motor.')
