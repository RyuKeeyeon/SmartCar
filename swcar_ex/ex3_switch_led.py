from ctypes import*
import os
import time

WiringPi = CDLL("/home/pi/WiringPi/wiringPi/libwiringPi.so.2.70", mode=RTLD_GLOBAL)
swcar = cdll.LoadLibrary('/home/pi/swcar_lib/librp_smartcar.so')

swcar.SIO_Init(0)

print('Press ctrl + c to terminate program')
print('push switch if you want.')

switch = 0;
switch_pre = 0;

swcar.SIO_WriteLED(0)

high = 0;
new  = 0;

while(True):

    switch = swcar.SIO_ReadSwitch()

    if   ((switch // 1 % 2) == 0):
        new = 1;
    elif ((switch // 2 % 2) == 0):
        new = 2;
    elif ((switch // 4 % 2) == 0):
        new = 3;
    else:
        new = 0;
        
    if (new > 0):
        swcar.SIO_WriteLED(high * 10 + new)
        high = new
        new = 0
        
    time.sleep(0.1)
