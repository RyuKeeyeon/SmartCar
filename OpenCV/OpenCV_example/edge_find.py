import cv2

cap = cv2.VideoCapture(0)
while (cap.isOpened()):
  ret,img = cap.read()
  cv2.imshow('my win',img)
  img_canny = cv2.Canny(img, 50, 150) #엣지 검출 정도 변경을 원할시에 함수 입력값 조정
  cv2.imshow('canny',img_canny)
  if cv2.waitKey(1) & 0xFF == ord('q'):
    break
cap.release()
cv2.destroyAllWindows()
