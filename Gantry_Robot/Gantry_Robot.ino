
// CNC Shield Stepper  Control Demo
// Superb Tech
// www.youtube.com/superbtech
const byte enablePin = 8;
const int StepX = 2;
const int DirX = 5;

const int StepY = 3;
const int DirY = 6;

const int StepZ = 12;
const int DirZ = 13;

const int LimitX = 9;
const int LimitY = 10;

int x_pos = 0;
int y_pos = 0;
int z_pos = 0;


int stepping_mode = 1/(0.5); //Change the value in the brackets to change the stepping mode

const int x_soft_limit = 1800 * stepping_mode;
const int y_soft_limit = 1500 * stepping_mode;

int home_is_busy = 0;

const int norm_speed = 800; //Delay in microseconds between steps
const int home_speed = 10000;
const int numSteps_WASD = 200; //Manual homing precision

void setup() {
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW);  // enable steppers
  //Set the pins connected to the stepper motor drivers
  pinMode(StepX,OUTPUT);
  pinMode(DirX,OUTPUT);
  
  pinMode(StepY,OUTPUT);
  pinMode(DirY,OUTPUT);

  pinMode(StepZ,OUTPUT);
  pinMode(DirZ,OUTPUT);
  // Set the limit switch pins as inputs
  pinMode(LimitX, INPUT);
  pinMode(LimitY, INPUT);

  Serial.begin(9600);
  while (!Serial) {
    // Wait for the serial port to connect. Needed for Leonardo only
  }
  // Serial.println("Serial communication started. Send a message:");
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
        moveStepper(StepX, DirX, x_steps, HIGH, norm_speed, x_pos);
      }else{
        moveStepper(StepX, DirX, abs(x_steps), LOW, norm_speed, x_pos);
      }

      if (y_steps >= 0){
        moveStepper(StepY, DirY, y_steps, HIGH, norm_speed, y_pos);
      }else{
        moveStepper(StepY, DirY, abs(y_steps), LOW, norm_speed, y_pos);
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
        moveStepper(StepX, DirX, x_steps, HIGH, norm_speed, x_pos);
      }else{
        moveStepper(StepX, DirX, abs(x_steps), LOW, norm_speed, x_pos);
      }

      if (y_steps >= 0){
       moveStepper(StepY, DirY, y_steps, HIGH, norm_speed, y_pos);
      }else{
       moveStepper(StepY, DirY, abs(y_steps), LOW, norm_speed, y_pos);
      }
    }

    if (command == 'a'){
      moveStepper(StepX, DirX, numSteps_WASD, LOW, norm_speed, x_pos);
    }  
    
    if (command == 'd') {
      moveStepper(StepX, DirX, numSteps_WASD, HIGH, norm_speed, x_pos);
    }

    if (command == 'w') {
      moveStepper(StepY, DirY, numSteps_WASD, HIGH, norm_speed, y_pos);
    }
      
    if (command == 's') {
      moveStepper(StepY, DirY, numSteps_WASD, LOW, norm_speed, y_pos);
    }

    if (command == 'o') {
      Serial.println("O has been pressed.");
      moveStepper(StepZ, DirZ, 300, HIGH, 500, z_pos);
    }

    if (command == 'l') {
      Serial.println("L has been pressed.");
      moveStepper(StepZ, DirZ, 300, LOW, 500, z_pos);
    }

    if (command == 'h'){
      home_is_busy = 1;
      //Home the X-Axes
      // Read the state of the limit X switch
      int LimitX_state = digitalRead(LimitX); 
      while(LimitX_state != HIGH){  
        moveStepper(StepX, DirX, 1, LOW, norm_speed, x_pos);
        LimitX_state = digitalRead(LimitX);
      }
      //Move back a few steps
      moveStepper(StepX, DirX, 50, HIGH, norm_speed, x_pos);
      //Move Slowly back to home
      LimitX_state = digitalRead(LimitX); 
      while(LimitX_state != HIGH){  
        moveStepper(StepX, DirX, 1, LOW, home_speed, x_pos);
        LimitX_state = digitalRead(LimitX);
      }
      //Move back a few steps
      moveStepper(StepX, DirX, 10, HIGH, home_speed, x_pos);


      // Home the Y-axes
      // Read the state of the limit Y switch
      int LimitY_state = digitalRead(LimitY);
      while(LimitY_state != HIGH){  
        moveStepper(StepY, DirY, 1, LOW, norm_speed, y_pos);
        LimitY_state = digitalRead(LimitY);
      }
      // Move back a few steps
      moveStepper(StepY, DirY, 50, HIGH, norm_speed, y_pos);

      LimitY_state = digitalRead(LimitY);
      while(LimitY_state != HIGH){  
        moveStepper(StepY, DirY, 1, LOW, home_speed, y_pos);
        LimitY_state = digitalRead(LimitY);
      }
      // Move back a few steps
      moveStepper(StepY, DirY, 10, HIGH, home_speed, y_pos);

      y_pos = 0;
      x_pos = 0;

      home_is_busy = 0;
    }

    if (command == 'p'){
      Serial.print("X pos: ");
      Serial.println(x_pos);
      Serial.print("Y pos: ");
      Serial.println(y_pos);
    }
  }
}

void moveStepper(int stepPin, int dirPin, int steps, int direction, int speed, int &position) {
  // Set the direction of the stepper motor
  digitalWrite(dirPin, direction);

  // Move the stepper motor the specified number of steps
  for (int i = 0; i < steps; i++) {
    // if (stepPin == StepX){
    //     if (direction == HIGH && position >= x_soft_limit ) {
    //       return;
    //     }
    // } else {
    //     if (direction == HIGH && position >= y_soft_limit ) {
    //       return;
    //     } 
    // }
    
    // if (direction == LOW && position <= 0 ) {
    //   if (home_is_busy == 0){
    //     return;
    //   }      
    // }

    digitalWrite(stepPin, HIGH);
    delayMicroseconds(speed);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(speed);
    
    // Update the position based on direction
    if (direction == HIGH) {
      position += 1;  // Moving forward
    } else {
      position -= 1;  // Moving backward
    }
  }
}

