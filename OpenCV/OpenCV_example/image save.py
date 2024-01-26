import cv2

img = cv2.imread(r"/OpenCV/OpenCV_example/face.jpg", cv2.IMREAD_COLOR)
cv2.imshow('my win',img)
key = cv2.waitKey(0)&0xff
if key != 27:
  cv2.imwrite(r'/OpenCV/OpenCV_example/face2.jpg', img)
cv2.destroyAllWindows()
