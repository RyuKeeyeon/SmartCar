import cv2
import time

print(cv2.__version__)

c = cv2.VideoCapture(0,cv2.CAP_V4L)
time.sleep(0.1)

c.set(cv2.CAP_PROP_FRAME_WIDTH,2560);
c.set(cv2.CAP_PROP_FRAME_HEIGHT,720);

while(True):
    _,image = c.read()
    image = cv2.resize(image, (1280,360))
    cv2.imshow("Frame", image)
    cv2.waitKey(1)
    
cv2.destroyAllWindows()


