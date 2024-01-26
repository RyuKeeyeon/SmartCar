import time
from ctypes import *


class car:

    def __init__(self):
        # Load the WiringPi library for Raspberry Pi GPIO and the smart car library
        self.wirePIfunc = CDLL(r"C:\SmartCar\WiringPi\wiringPi\libwiringPi.so.2.70", mode=RTLD_GLOBAL)
        self.swcarfunc = cdll.LoadLibrary(r'C:\SmartCar\swcar_lib\librp_smartcar.so')

        # Initialize the smart car library and set the maximum motor speed
        self.swcarfunc.SIO_Init(0)
        self.swcarfunc.SIO_MaxMotorSpeed(50)

        # Stop the motor, center the servo, and wait for 2 seconds
        self.motor_stop()
        self.servo_center()
        time.sleep(2)

        # Set initial statuses for motor and servo
        self.motor_status = "STOP"
        self.servo_status = "CENTER"

    def motor_forward(self):
        # Set the speed for forward motion
        speed = 20
        if (self.motor_status == "REVERSE"):
            # If the motor is in reverse, stop it first
            self.swcarfunc.SIO_ForwardMotor(0)
            time.sleep(1)
        # Start forward motion
        self.swcarfunc.SIO_ForwardMotor(speed)
        self.motor_status = "FORWARD"

    def motor_reverse(self):
        # Set the speed for reverse motion
        speed = 20
        if (self.motor_status == "FORWARD"):
            # If the motor is in forward, stop it first
            self.swcarfunc.SIO_ReverseMotor(0)
            time.sleep(1)
        # Start reverse motion
        self.swcarfunc.SIO_ReverseMotor(speed)
        self.motor_status = "REVERSE"

        # Reset rear ultrasonic sensor distance and infrared sensor position
        self.sensor_dist_rear = 0
        self.sensor_pos_ir = 0

    def motor_stop(self):
        # Stop the motor
        self.swcarfunc.SIO_ForwardMotor(100, 0)
        self.motor_status = "STOP"

    def servo_left(self):
        # Turn the servo to the left
        self.swcarfunc.SIO_WriteServo(100, 90)
        self.servo_status = "LEFT"

    def servo_right(self):
        # Turn the servo to the right
        self.swcarfunc.SIO_WriteServo(100, 10)
        self.servo_status = "RIGHT"

    def servo_center(self):
        # Center the servo
        self.swcarfunc.SIO_WriteServo(100, 50)
        self.servo_status = "CENTER"

    def sensor_get_distance_US_REAR(self):
        # Read the distance from the rear ultrasonic sensor
        self.sensor_dist_rear = self.swcarfunc.SIO_ReadDistUS(0)
        return self.sensor_dist_rear

    def sensor_get_position_IR(self):
        # Read the position from the infrared sensor
        self.sensor_pos_ir = self.swcarfunc.SIO_ReadIR()
        return self.sensor_pos_ir


if __name__ == '__main__':
    # Create an instance of the car class
    mycar = car()

    while (True):
        print("##")

        # Read sensor values
        sens_IR = mycar.sensor_get_position_IR()
        sens_rear = mycar.sensor_get_distance_US_REAR()

        # Print sensor values
        print("IR=", format(sens_IR, 'b'))
        print("US rear=", format(sens_rear, 'd'))

        # Move the car forward with right turn, then reverse with left turn
        mycar.servo_right()
        mycar.motor_forward()
        time.sleep(1)

        mycar.servo_left()
        mycar.motor_reverse()
        time.sleep(1)

    # This code will never be reached due to the infinite loop
    print("exit.")
    exit()
