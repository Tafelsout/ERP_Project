import serial
import time

# Initialize the serial connection (update '/dev/ttyUSB0' with your serial port if needed)
ser = serial.Serial('/dev/ttyACM0', 9600, timeout=10)

time.sleep(2)  # Wait for the connection to establish

while True:
    # Ask the user what to send to Arduino
    user_input = input("Enter the message to send to Arduino: ")
    
    # Send the user's input to the Arduino
    ser.write(user_input.encode('utf-8'))
    
    # Give Arduino some time to respond (optional, depending on your application)
    time.sleep(1000)
    
    # Read the response from Arduino if any
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').rstrip()
        print(f'Received from Arduino: {line}')

