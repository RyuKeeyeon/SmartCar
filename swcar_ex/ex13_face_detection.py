import cv2

# Load the cascade
face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')

# Read the input image
img = cv2.imread('/home/bosco/pi/swcar_ex/lenna.png')  # Replace with the correct image path
# Check if the image is loaded successfully
if img is None:
    print("Error: Unable to load the image.")
else:
    # Convert into grayscale
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    # Detect faces
    faces = face_cascade.detectMultiScale(gray, 1.1, 4)
    # Draw rectangle around the faces
    for (x, y, w, h) in faces:
        cv2.rectangle(img, (x, y), (x+w, y+h), (255, 0, 0), 2)
    # Save the output image
    cv2.imwrite('/home/bosco/pi/swcar_ex/output_image.png', img)
