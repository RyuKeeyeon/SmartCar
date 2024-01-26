import cv2

print(cv2.__version__)

img = cv2.imread("./lenna.png")

winname = "test"
cv2.namedWindow(winname)   # create a named window
cv2.moveWindow(winname, 600, 200)   # Move it to (600, 200)
cv2.imshow(winname, img)

cv2.waitKey()
cv2.destroyAllWindows()

