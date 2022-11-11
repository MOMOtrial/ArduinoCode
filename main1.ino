
// Include the motor shield V1 library
#include <AFMotor.h>

// Define the analog pins used by the FSR and push buttons
#define fsrpin A0
#define pushButton1 A1
#define pushButton2 A2
#define pushButton3 A3
#define pushButton4 A4
#define pushButton5 A5

// Define booleans to store the states of the push buttons and test variables
boolean pushButtonState1 = HIGH;
boolean pushButtonState2 = HIGH;
boolean pushButtonState3 = HIGH;
boolean pushButtonState4 = HIGH;
boolean pushButtonState5 = HIGH;
boolean test;
boolean stepperForwards = true;

//Define variable to store FSR readings and the number of steps taken
int fsrreading;
int steps = 0;

// Connect a stepper motor with 2038 steps per revolution to motor port #2 (M3 and M4)
AF_Stepper stepperMotor(2038, 2);

// Connect the DC motor to M1 on the motor control board
AF_DCMotor DCmotor(1, MOTOR12_1KHZ);
AF_DCMotor DCmotor2(2, MOTOR12_1KHZ);

void setup() {
  // Set up serial communication to print things to the serial monitor at 9600 bps
  Serial.begin(9600);

  // Set the pushButton to being input, with a pullup resistor connected
  pinMode(pushButton1, INPUT_PULLUP);
  pinMode(pushButton2, INPUT_PULLUP);
  pinMode(pushButton3, INPUT_PULLUP);
  pinMode(pushButton4, INPUT_PULLUP);
  pinMode(pushButton5, INPUT_PULLUP);
  
  // Set the motor speed in rpm - generally anything more than 25 is too fast for the
  // motor to run smoothly 
  stepperMotor.setSpeed(15);

  // Start the DC motor off still and set the speed
  DCmotor.run(RELEASE);
  DCmotor.setSpeed(225);
  
  DCmotor2.run(RELEASE);
  DCmotor2.setSpeed(225);
}

void loop() {
  // Read the state of the three push buttons
  pushButtonState1=digitalRead(pushButton1);
  pushButtonState2=digitalRead(pushButton2);
  pushButtonState3=digitalRead(pushButton3);
  pushButtonState4=digitalRead(pushButton4);
  pushButtonState5=digitalRead(pushButton5);

  // Turn the stepper forwards until the FSR detects any pressure 
  if (pushButtonState1==LOW && stepperForwards==true){
    test = true; // Set test to true for the while loop to run
    Serial.println("Stepper moving forwards");
    while (test==true){
      stepperMotor.step(10, FORWARD, DOUBLE); // Take 10 steps then add 10 to the steps variable
      steps += 25;
      fsrreading = analogRead(fsrpin);
      if (fsrreading > 100){
        Serial.println("Stepper stopped");
        Serial.print("FSR reading:");
        Serial.println(fsrreading); 
        Serial.print("Steps taken:");
        Serial.println(steps);
        test = false; // Stop the while loop now that enough pressure has been detected
        stepperForwards = false; // Tell the sketch to skip to the backwards loop until the button has been pressed again
      }
    }
  }
  else if (pushButtonState1==LOW){
    Serial.println("Stepper moving backwards");
    stepperMotor.step(steps, BACKWARD, DOUBLE);
    stepperForwards = true;
    steps = 0;

    // This next function is important as it stops current from going through the stepper motor when you don't need the motor
    // to hold its position. This should be included any time you don't need the stepper to remain fixed. 
    stepperMotor.release();  
  }

  if (pushButtonState2 == LOW) { 
    Serial.println("DC1 motor moving forwards");
    DCmotor.run(FORWARD);
    delay(10);
  }
  else if (pushButtonState3 == LOW) { 
    Serial.println("DC1 motor moving backwards");
    DCmotor2.run(FORWARD);
    delay(10);
  }
  else if (pushButtonState4 == LOW) { 
    Serial.println("DC2 motor moving forwards");
    DCmotor.run(BACKWARD);
    delay(10); 
  }
  else if (pushButtonState5 == LOW) { 
    Serial.println("DC2 motor moving backwards");
    DCmotor2.run(BACKWARD);
    delay(10); 
  }
  else{
    DCmotor.run(RELEASE);
    DCmotor2.run(RELEASE);
  }
}
