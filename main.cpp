#include <Arduino.h>

const int SERVO_PIN = 14;     // ServoPIN
const int SERVO_CH = 0;       // PWMChannel
const int SERVO_FREQ = 50;    
const int SERVO_RES = 12;     

const int MIN_US = 500;       
const int MAX_US = 2500;      

const int OPEN_POS = 90;      //OPEN_ANGLE
const int CLOSE_POS = 0;      //CLOSE_ANGLE

bool doorOpen = false;        //STATE

// แปลงองศา to pulse width
int angleToMicros(int angle) {
  angle = constrain(angle, 0, 180);
  return MIN_US + ((long)(MAX_US - MIN_US) * angle) / 180;
}

//convert pulse width to duty cycle
int microsToDuty(int us) {
  us = constrain(us, MIN_US, MAX_US);
  const int maxCount = (1 << SERVO_RES) - 1; // 4095
  long duty = ((long)us * maxCount) / 20000L; // 20ms period
  return (int)constrain(duty, 0, maxCount);
}

// function for send angle to Servo
void writeAngle(int angleDeg) {
  int us = angleToMicros(angleDeg);
  int duty = microsToDuty(us);
  ledcWrite(SERVO_CH, duty);
}

// OPEN DOOR FUNCTION
void openDoor() {
  writeAngle(OPEN_POS);
  doorOpen = true;
  Serial.println("Door Opened");
}

// CLOSE DOOR FUNCTION
void closeDoor() {
  writeAngle(CLOSE_POS);
  doorOpen = false;
  Serial.println("Door Closed");
}

void setup() {
  Serial.begin(9600);
  ledcSetup(SERVO_CH, SERVO_FREQ, SERVO_RES);
  ledcAttachPin(SERVO_PIN, SERVO_CH);

 
  closeDoor(); //call close door function
}

void loop() {
  // ========================================  //
  // Ultrasonic and UART Part                 //
  // ========================================//
  if (Serial.available() > 0) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd == "OPEN") {
      openDoor();
      delay(5000);  
      closeDoor();
    } 
    else if (cmd == "CLOSE") {
      closeDoor();
    }
  }
}
