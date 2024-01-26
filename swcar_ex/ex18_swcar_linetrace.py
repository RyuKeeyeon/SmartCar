import sys
sys.path.append("/home/pi/swcar_ex/")

from ex17_swcar_test import car
import time

from PyQt5.QtWidgets import *
from PyQt5.QtCore import *

class Thread1(QThread):
    def __init__(self, parent = None):
        super().__init__(parent)
        
    def run(self):
        #Line Tracing
        mycar = car()
        goal = False
        
        while(goal == False) :
            
            # Collision Detection
            dist = mycar.sensor_get_distance_US_REAR()
            
            if (dist < 150) :
                avoid_collision = 1
            else :
                avoid_collision = 0
                
            # Read Line Position & Control Handle
            pos = mycar.sensor_get_position_IR()
            if (pos == 0b1001) :
                mycar.servo_center()
            elif (pos == 0b0011) or (pos == 0b0001) :
                mycar.servo_left()
            elif (pos == 0b1100) or (pos == 0b1000) :
                mycar.servo_right()
            elif (pos == 0b0000) :
                # out of lane : move backward until lane detected.
                mycar.servo_center()
                mycar.motor_reverse()
                while(pos == 0b0000) :
                    pos = mycar.sensor_get_position_IR()
                mycar.motor_stop()
                time.sleep(1)
            elif (pos == 0b1111) :
                # goal
                mycar.servo_center()
                goal = True
                
            # Motor Driving
            if (goal == True) :
                mycar.motor_stop()
                print("Goal Line!!")
            elif (avoid_collision == 1) :
                mycar.motor_stop()
                print("Stop to avoid collision : US_rear(%d)" % (dist))
                time.sleep(1)
            else:
                print("sensor : US_rear(%d)" % (dist))
                mycar.motor_forward()
            
class MainWidget(QWidget):
    def __init__(self):
        super().__init__()
        self.thread_start = QPushButton("Start Line Tracer")
        self.thread_start.clicked.connect(self.linetracer)
        
        vbox = QVBoxLayout()
        vbox.addWidget(self.thread_start)
        
        self.resize(200,200)
        self.setLayout(vbox)
        
        self.threadrun = False
        
    def linetracer(self):
        if (self.threadrun == False) :
            self.x=Thread1(self)
            self.x.start()
            self.threadrun = True
            self.thread_start.setText("Running")
            
if __name__ == '__main__':
    app = QApplication(sys.argv)
    widget = MainWidget()
    widget.show()
    sys.exit(app.exec_())