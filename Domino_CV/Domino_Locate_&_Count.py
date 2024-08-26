import cv2
import numpy as np

# Camera Settings
dispW = 640
dispH = 480
flip = 2
camSet='nvarguscamerasrc !  video/x-raw(memory:NVMM), width=3264, height=2464, format=NV12, framerate=21/1 ! nvvidconv flip-method='+str(flip)+' ! video/x-raw, width='+str(dispW)+', height='+str(dispH)+', format=BGRx ! videoconvert ! video/x-raw, format=BGR ! appsink'
cam = cv2.VideoCapture(camSet)

# Disable auto exposure and set fixed exposure
cam.set(cv2.CAP_PROP_AUTO_EXPOSURE, 0.25)  # 0.25 sets manual exposure mode
cam.set(cv2.CAP_PROP_EXPOSURE, -5)         # Set exposure level (-5 is just an example, adjust as needed)
cam.set(cv2.CAP_PROP_GAIN, 0)              # Set gain (0 disables automatic gain control)

def preprocess_frame(frame):
    gray_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    blurred_frame = cv2.GaussianBlur(gray_frame, (5, 5), 0)
    ave_pix_intesity = np.mean(blurred_frame)
    _, binary_frame = cv2.threshold(blurred_frame, ave_pix_intesity, 255, cv2.THRESH_BINARY_INV)
    return binary_frame

def find_domino_pieces(binary_frame):
    contours, _ = cv2.findContours(binary_frame, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    dominoes = []
    for contour in contours:
        approx = cv2.approxPolyDP(contour, 0.05 * cv2.arcLength(contour, True), True)
        if len(approx) == 4:
            x, y, w, h = cv2.boundingRect(approx)
            aspect_ratio = float(w) / h
            if 0.4 < aspect_ratio < 2.3:
                area = cv2.contourArea(contour)
                if 100 < area < 15000:
                    dominoes.append((x, y, w, h))
    return dominoes

def split_and_count_pips(frame, x, y, w, h):
    # Split the domino into two halves
    half_width = w // 2
    left_half = frame[y:y+h, x:x+half_width]
    right_half = frame[y:y+h, x+half_width:x+w]
    
    # Count pips in each half
    left_pips = count_pips(left_half)
    right_pips = count_pips(right_half)
    
    return left_pips, right_pips

def count_pips(half_frame):
    # Convert the half frame to grayscale and apply threshold to isolate white pips
    gray_half = cv2.cvtColor(half_frame, cv2.COLOR_BGR2GRAY)
    _, thresh_half = cv2.threshold(gray_half, 70, 255, cv2.THRESH_BINARY)
    
    # Find contours in the thresholded image
    contours, _ = cv2.findContours(thresh_half, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    
    pip_count = 0
    for contour in contours:
        # Use circularity to filter out non-pip contours
        area = cv2.contourArea(contour)
        perimeter = cv2.arcLength(contour, True)
        if perimeter == 0:
            continue
        circularity = 4 * np.pi * (area / (perimeter * perimeter))
        
        # Consider only contours that are nearly circular
        if 0.1 < circularity < 3:  # Adjust these values as needed
            pip_count += 1
    
    return pip_count

def draw_domino_pieces(frame, dominoes, binary_frame):
    for (x, y, w, h) in dominoes:
        # Draw bounding box around the detected domino piece
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
        
        # Calculate the center of the domino
        center_x = x + w // 2
        center_y = y + h // 2
        
        # Draw the center point
        cv2.circle(frame, (center_x, center_y), 5, (255, 0, 0), -1)
        
        # Split and count pips on each half
        left_pips, right_pips = split_and_count_pips(frame, x, y, w, h)
        
        # Display the pip counts on the frame
        cv2.putText(frame, f'{left_pips}', (x + 10, y + h // 2), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (255, 0, 0), 2)
        cv2.putText(frame, f'{right_pips}', (x + w - 30, y + h // 2), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (255, 0, 0), 2)
        
        # Print the pip counts
        print(f'Domino ({x}, {y}, {w}, {h}) - Left: {left_pips}, Right: {right_pips}')
    
    return frame

while True:
    ret, pi_frame = cam.read()
    if not ret:
        break
    
    binary_frame = preprocess_frame(pi_frame)
    dominoes = find_domino_pieces(binary_frame)
    output_frame = draw_domino_pieces(pi_frame, dominoes, binary_frame)
    
    # cv2.imshow('Original Frame', pi_frame)
    cv2.imshow('Binary Frame', binary_frame)
    cv2.imshow('Dominoes Detected', output_frame)
    
    if cv2.waitKey(1) == ord('q'):
        break

cam.release()
cv2.destroyAllWindows()
