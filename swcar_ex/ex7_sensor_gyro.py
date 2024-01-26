from ctypes import*
import os
import time

WiringPi = CDLL("/home/pi/WiringPi/wiringPi/libwiringPi.so.2.70", mode=RTLD_GLOBAL)
swcar = cdll.LoadLibrary('/home/pi/swcar_lib/librp_smartcar.so')

swcar.SIO_Init(0)

print('Press ctrl + c to terminate program')

swcar.SIO_ReadGyroAccel.restype = c_char_p
swcar.SIO_ReadGyroAccel.argtype = [POINTER(c_int), POINTER(c_int), POINTER(c_int)]
swcar.SIO_ReadGyroRotate.restype = c_char_p
swcar.SIO_ReadGyroRotate.argtype = [POINTER(c_int), POINTER(c_int), POINTER(c_int)]

acc_x = pointer(c_int(0))
acc_y = pointer(c_int(0))
acc_z = pointer(c_int(0))
rot_x = pointer(c_int(0))
rot_y = pointer(c_int(0))
rot_z = pointer(c_int(0))

while(True):
    
    swcar.SIO_ReadGyroAccel(acc_x, acc_y, acc_z)
    swcar.SIO_ReadGyroRotate(rot_x, rot_y, rot_z)
    
    print("acc = ", acc_x[0], acc_y[0], acc_z[0])
    print("rot = ", rot_x[0], rot_y[0], rot_z[0])
    
    time.sleep(1)
    
    