import time
from ctypes import *

class car:
 
    def __init__(self):
        self.wirePIfunc = CDLL("/home/pi/WiringPi/wiringPi/libwiringPi.so.2.70", mode=RTLD_GLOBAL)
        self.swcarfunc = cdll.LoadLibrary('/home/pi/swcar_lib/librp_smartcar.so')
        self.swcarfunc.SIO_Init(0)
        self.swcarfunc.SIO_MaxMotorSpeed(50)
        
        self.motor_stop()
        self.servo_center()
        time.sleep(2)    

        self.motor_status = "STOP"
        self.servo_status = "CENTER"
        
    def motor_forward(self):
        speed = 20
        if (self.motor_status == "REVERSE") :
            #Normal
            self.swcarfunc.SIO_ForwardMotor(0)
            time.sleep(1)
        #Forward    
        self.swcarfunc.SIO_ForwardMotor(speed)
        self.motor_status = "FORWARD"
    
    def motor_reverse(self):
        speed = 20
        if (self.motor_status == "FORWARD") :
            #Normal
            self.swcarfunc.SIO_ReverseMotor(0)
            time.sleep(1)
        #Reverse
        self.swcarfunc.SIO_ReverseMotor(speed)
        self.motor_status = "REVERSE"
        
        self.sensor_dist_rear = 0
        self.sensor_pos_ir = 0
        
    def motor_stop(self):
        self.swcarfunc.SIO_ForwardMotor(100, 0)
        self.motor_status = "STOP"
        
    def servo_left(self):
        self.swcarfunc.SIO_WriteServo(100, 90)
        self.servo_status = "LEFT"
        
    def servo_right(self):
        self.swcarfunc.SIO_WriteServo(100, 10)
        self.servo_status = "RIGHT"
        
    def servo_center(self):
        self.swcarfunc.SIO_WriteServo(100, 50)
        self.servo_status = "CENTER"
        
    def sensor_get_distance_US_REAR(self):
        self.sensor_dist_rear = self.swcarfunc.SIO_ReadDistUS(0)
        return self.sensor_dist_rear

    def sensor_get_position_IR(self):
        self.sensor_pos_ir = self.swcarfunc.SIO_ReadIR()
        return self.sensor_pos_ir

if __name__ == '__main__':
    
    mycar = car()
    
    while (True):
        print("##")
        
        sens_IR = mycar.sensor_get_position_IR()
        sens_rear = mycar.sensor_get_distance_US_REAR()
        
        print("IR=",format(sens_IR,'b'))
        print("US rear=",format(sens_rear,'d'))
        #time.sleep(1)
        
        mycar.servo_right()
        mycar.motor_forward()
        time.sleep(1)
         
        mycar.servo_left()
        mycar.motor_reverse()
        time.sleep(1)
        
    print("exit.")  
    exit()