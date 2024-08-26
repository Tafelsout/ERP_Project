import cv2
import numpy as np

# Camera Settings
dispW = 800
dispH = 800
flip = 2
camSet='nvarguscamerasrc !  video/x-raw(memory:NVMM), width=3264, height=2464, format=NV12, framerate=21/1 ! nvvidconv flip-method='+str(flip)+' ! video/x-raw, width='+str(dispW)+', height='+str(dispH)+', format=BGRx ! videoconvert ! video/x-raw, format=BGR ! appsink'
cam = cv2.VideoCapture(camSet)

# Disable auto exposure and set fixed exposure
cam.set(cv2.CAP_PROP_AUTO_EXPOSURE, 0.25)  # 0.25 sets manual exposure mode
cam.set(cv2.CAP_PROP_EXPOSURE, -5)         # Set exposure level (-5 is just an example, adjust as needed)

# Optionally, you can also set a fixed gain
cam.set(cv2.CAP_PROP_GAIN, 0)              # Set gain (0 disables automatic gain control)

def preprocess_frame(frame):
    # Convert to grayscale
    gray_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    
    # Apply Gaussian Blur to reduce noise and improve contour detection
    blurred_frame = cv2.GaussianBlur(gray_frame, (5, 5), 0)
    # blurred_frame = gray_frame
    
    # Binarize the image: Black dominoes will be 0, white background will be 255
    ave_pix_intesity = np.mean(blurred_frame)
    _, binary_frame = cv2.threshold(blurred_frame, ave_pix_intesity-10, 255, cv2.THRESH_BINARY_INV)
    # print(binary_frame.shape())
    return binary_frame

def find_domino_pieces(binary_frame):
    # Find contours in the binary image
    contours, _ = cv2.findContours(binary_frame, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    
    dominoes = []
    for contour in contours:
        # Approximate the contour to a polygon to reduce the number of points
        approx = cv2.approxPolyDP(contour, 0.05 * cv2.arcLength(contour, True), True)
        
        # If the approximated contour has 4 points, it could be a domino piece
        if len(approx) == 4:
            # Get the bounding box of the contour
            x, y, w, h = cv2.boundingRect(approx)
            
            # Check if the aspect ratio is consistent with a domino piece
            aspect_ratio = float(w) / h
            if 0.4 < aspect_ratio < 2.3:  # Adjust these values as necessary
                # Calculate the area of the contour and filter by size
                area = cv2.contourArea(contour)
                if 100 < area < 15000:  # Adjust these values based on your expected domino size
                    dominoes.append((x, y, w, h))
    
    return dominoes

def draw_domino_pieces(frame, dominoes):
    for (x, y, w, h) in dominoes:
        # Draw bounding box around the detected domino piece
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
        
        # Calculate the center of the domino
        center_x = x + w // 2
        center_y = y + h // 2
        
        # Draw the center point
        cv2.circle(frame, (center_x, center_y), 5, (255, 0, 0), -1)
        
        # Put the text of the center coordinates on the frame
        cv2.putText(frame, f'({center_x}, {center_y})', (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 0, 0), 2)
        
        # Print the center coordinates
        print(f'Domino center at: ({center_x}, {center_y})')
    
    return frame

while True:
    ret, pi_frame = cam.read()
    if not ret:
        break
    
    binary_frame = preprocess_frame(pi_frame)
    dominoes = find_domino_pieces(binary_frame)
    output_frame = draw_domino_pieces(pi_frame, dominoes)
    
    # cv2.imshow('Original Frame', pi_frame)
    cv2.imshow('Binary Frame', binary_frame)
    cv2.imshow('Dominoes Detected', output_frame)
    
    if cv2.waitKey(1) == ord('q'):
        break

cam.release()
cv2.destroyAllWindows()
