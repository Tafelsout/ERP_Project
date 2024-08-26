
// CNC Shield Stepper  Control Demo
// Superb Tech
// www.youtube.com/superbtech

const int StepX = 2;
const int DirX = 5;

const int StepY = 3;
const int DirY = 6;

const int StepZ = 4;
const int DirZ = 7;

int x_pos = 0;
int y_pos = 0;

const int speed = 1500; //Delay between steps
const int numSteps_WASD = 100; //Manual homing precision

void setup() {
  pinMode(StepX,OUTPUT);
  pinMode(DirX,OUTPUT);
  pinMode(StepY,OUTPUT);
  pinMode(DirY,OUTPUT);
  pinMode(StepZ,OUTPUT);
  pinMode( DirZ,OUTPUT);

  Serial.begin(9600);
  while (!Serial) {
    // Wait for the serial port to connect. Needed for Leonardo only
  }
  Serial.println("Serial communication started. Send a message:");
}


void loop() {

  if (Serial.available() > 0) {
    // Read the incoming data as a string
    String input = Serial.readStringUntil('\n');
    input.trim(); // Remove any leading or trailing whitespace

    // Extract command and numbers
    char command = input.charAt(0); // Extract the command (e.g., 'M')

    if (command == 'r'){
      int space1 = input.indexOf(' ');
      int space2 = input.indexOf(' ', space1 + 1);  
      // Extract the X and Y values
      String xString = input.substring(space1 + 1, space2);
      String yString = input.substring(space2 + 1);
      // Convert strings to integers
      int x_steps = xString.toInt();
      int y_steps = yString.toInt();

      // Send the steps to the X_Y stepper motors
      if (x_steps >= 0){
        digitalWrite(DirX, HIGH);
        for(int x = 0; x < x_steps; x++) { 
          digitalWrite(StepX,HIGH);
          delayMicroseconds(speed);
          digitalWrite(StepX,LOW); 
          delayMicroseconds(speed);
          x_pos = x_pos + 1;
        }
      }else{
        digitalWrite(DirX, LOW);
        x_steps = abs(x_steps);
        for(int x = 0; x < x_steps; x++) { 
          digitalWrite(StepX,HIGH);
          delayMicroseconds(speed);
          digitalWrite(StepX,LOW); 
          delayMicroseconds(speed);
          x_pos = x_pos - 1;
        }
      }

      if (y_steps >= 0){
       digitalWrite(DirY, LOW);
       for(int x = 0; x < y_steps; x++) { 
         digitalWrite(StepY,HIGH);
         delayMicroseconds(speed);
         digitalWrite(StepY,LOW); 
         delayMicroseconds(speed);
         y_pos = y_pos + 1;
       }
      }else{
        digitalWrite(DirY, HIGH);
        y_steps = abs(y_steps);
        for(int x = 0; x < y_steps; x++) { 
          digitalWrite(StepY,HIGH);
          delayMicroseconds(speed);
          digitalWrite(StepY,LOW); 
          delayMicroseconds(speed);
          y_pos = y_pos - 1;
          }
      }

    }

    if (command == 'u'){
      int space1 = input.indexOf(' ');
      int space2 = input.indexOf(' ', space1 + 1);  
      // Extract the X and Y values
      String xString = input.substring(space1 + 1, space2);
      String yString = input.substring(space2 + 1);
      // Convert strings to integers
      int x_uni_pos = xString.toInt();
      int y_uni_pos = yString.toInt();

      int x_steps = x_uni_pos - x_pos;
      int y_steps = y_uni_pos - y_pos;

      if (x_steps >= 0){
        digitalWrite(DirX, HIGH);
        for(int x = 0; x < x_steps; x++) { 
          digitalWrite(StepX,HIGH);
          delayMicroseconds(speed);
          digitalWrite(StepX,LOW); 
          delayMicroseconds(speed);
          x_pos = x_pos + 1;
        }
      }else{
        digitalWrite(DirX, LOW);
        x_steps = abs(x_steps);
        for(int x = 0; x < x_steps; x++) { 
          digitalWrite(StepX,HIGH);
          delayMicroseconds(speed);
          digitalWrite(StepX,LOW); 
          delayMicroseconds(speed);
          x_pos = x_pos - 1;
        }
      }

      if (y_steps >= 0){
       digitalWrite(DirY, LOW);
       for(int x = 0; x < y_steps; x++) { 
         digitalWrite(StepY,HIGH);
         delayMicroseconds(speed);
         digitalWrite(StepY,LOW); 
         delayMicroseconds(speed);
         y_pos = y_pos + 1;
       }
      }else{
        digitalWrite(DirY, HIGH);
        y_steps = abs(y_steps);
        for(int x = 0; x < y_steps; x++) { 
          digitalWrite(StepY,HIGH);
          delayMicroseconds(speed);
          digitalWrite(StepY,LOW); 
          delayMicroseconds(speed);
          y_pos = y_pos - 1;
          }
      }
    }

    if (command == 'a'){
      digitalWrite(DirX, LOW); // set direction, HIGH for clockwise, LOW for anticlockwise 
      for(int x = 0; x<numSteps_WASD; x++) { // loop for 200 steps
       digitalWrite(StepX,HIGH);
       delayMicroseconds(speed);
       digitalWrite(StepX,LOW); 
       delayMicroseconds(speed);
       x_pos = x_pos - 1;
      }
    }  
    
    if (command == 'd') {
      digitalWrite(DirX, HIGH);
      for(int x = 0; x<numSteps_WASD; x++) { // loop for 200 steps
        digitalWrite(StepX,HIGH);
        delayMicroseconds(speed);
        digitalWrite(StepX,LOW); 
        delayMicroseconds(speed);
        x_pos = x_pos + 1;
      }
    }

    if (command == 'w') {
      digitalWrite(DirY, LOW);
      for(int x = 0; x<numSteps_WASD; x++) { // loop for 200 steps
        digitalWrite(StepY,HIGH);
        delayMicroseconds(speed);
        digitalWrite(StepY,LOW); 
        delayMicroseconds(speed);
        y_pos = y_pos + 1;
      }
    }
      
    if (command == 's') {
      digitalWrite(DirY, HIGH);
      for(int x = 0; x<numSteps_WASD; x++) { // loop for 200 steps
        digitalWrite(StepY,HIGH);
        delayMicroseconds(speed);
        digitalWrite(StepY,LOW); 
        delayMicroseconds(speed);
        y_pos = y_pos - 1;
      }
    }

    if (command == 'h'){
      y_pos = 0;
      x_pos = 0;
    }

    if (command == 'p'){
      Serial.print("X pos: ");
      Serial.println(x_pos);
      Serial.print("Y pos: ");
      Serial.println(y_pos);
    }
    

  }
}
