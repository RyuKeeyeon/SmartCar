import sys
import time
import cv2

from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from PyQt5.QtGui import QImage, QPixmap, QColor

class ThreadCam(QThread):
    changePixmap = pyqtSignal(QImage)
    changePixmap2 = pyqtSignal(QImage)
      
    def run(self):
        cap = cv2.VideoCapture(0,cv2.CAP_V4L)
        while True:
            ret, frame = cap.read()
            frame = cv2.resize(frame, (320,240))
            frame2 = cv2.Canny(frame, 50, 150)
            
            if ret :
                # frame1
                rgbImage = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
                h,w,ch = rgbImage.shape
                bytesPerLine = ch * w
                convertToQtFormat = QImage(rgbImage.data, w,h, bytesPerLine, QImage.Format_RGB888)
                p=convertToQtFormat.scaled(320,240, Qt.KeepAspectRatio)
                self.changePixmap.emit(p)
                
                # frame2
                rgbImage2 = cv2.cvtColor(frame2, cv2.COLOR_BGR2RGB)
                h2,w2,ch2 = rgbImage2.shape
                bytesPerLine2 = ch2 * w2
                convertToQtFormat2 = QImage(rgbImage2.data, w2,h2, bytesPerLine2, QImage.Format_RGB888)
                p2=convertToQtFormat2.scaled(320,240, Qt.KeepAspectRatio)
                self.changePixmap2.emit(p2)
            
class MainWidget(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()
        
    def setImage(self,image):
        self.imgLabelCam.setPixmap(QPixmap.fromImage(image))
    
    def setImage2(self,image):
        self.imgLabelProcess.setPixmap(QPixmap.fromImage(image))
    
    def initUI(self):
        
        self.setWindowTitle("Simple GUI")
        #self.setGeometry(self.left, self.top, self.width, self.height)
        #self.resize(800,600)
        
        # create the label that holds the image
        self.imgLabelCam = QLabel(self)
        self.imgLabelProcess = QLabel(self)
        
        # create text label
        self.textLabelQuit = QLabel('Quit : ')
        
        # create hbox layout and add the two image labels
        hbox = QHBoxLayout()
        hbox.addStretch(1)
        hbox.addWidget(self.imgLabelCam)
        hbox.addWidget(self.imgLabelProcess)
        hbox.addStretch(1)
        
        # create the button
        self.btnQuit = QPushButton("Quit")
        self.btnQuit.clicked.connect(QCoreApplication.instance().quit)
        
        # create a vertical box layout and add the two labels
        vbox = QVBoxLayout()
        vbox.addStretch(3)
        vbox.addLayout(hbox)
        vbox.addStretch(1)
        
        vbox.addWidget(self.textLabelQuit)
        vbox.addWidget(self.btnQuit)
        
        # set the vbox layout as the widgets layout
        self.setLayout(vbox)     
      
        # create Camera Thread and start
        cam = ThreadCam(self)
        cam.changePixmap.connect(self.setImage)
        cam.changePixmap2.connect(self.setImage2)
        cam.start()
    
if __name__ == '__main__':

    app = QApplication(sys.argv)
    widget = MainWidget()
    widget.show()
    sys.exit(app.exec_())
