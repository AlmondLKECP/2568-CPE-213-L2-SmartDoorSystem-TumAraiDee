#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Servo.h>

// ==== CONFIG ====
#define SERVO_PIN 14
#define TRIG_PIN1 32
#define ECHO_PIN1 33
#define TRIG_PIN2 25
#define ECHO_PIN2 26
#define BUTTON_PIN 27   // ปุ่มออก

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Servo doorServo;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ==== PASSWORD ====
String correctPassword = "256574";
String inputPassword = "";
int attempts = 0;

// ==== STATE ====
bool doorOpen = false;

// =======================
long readUltrasonic(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 30000); // timeout 30ms
  return duration * 0.034 / 2; // cm
}

void openDoor() {
  doorServo.write(90);   // เปิด 90 องศา
  doorOpen = true;
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Door Opened (90)");
  display.display();
  delay(2000); // เวลาผ่านประตู
}

void closeDoor() {
  doorServo.write(0);   // ปิดกลับ
  doorOpen = false;
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Door Closed");
  display.display();
  delay(1000);
}

void showPasswordScreen() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Enter Password:");
  display.println(inputPassword);
  display.display();
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN1, OUTPUT);
  pinMode(ECHO_PIN1, INPUT);
  pinMode(TRIG_PIN2, OUTPUT);
  pinMode(ECHO_PIN2, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  doorServo.attach(SERVO_PIN);
  doorServo.write(0); // ปิดเริ่มต้น

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Smart Door Ready");
  display.display();
}

void loop() {
  long dist1 = readUltrasonic(TRIG_PIN1, ECHO_PIN1);
  long dist2 = readUltrasonic(TRIG_PIN2, ECHO_PIN2);

  // ==== ตรวจจับเข้าด้านหน้า (ต้องใส่รหัส) ====
  if (dist1 > 0 && dist1 < 30 && !doorOpen) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Enter Password:");
    display.display();

    inputPassword = "";
    attempts = 0;

    // ==== อ่านรหัสผ่านจาก Serial (PC keyboard) ====
    while (inputPassword != correctPassword && attempts < 3) {
      if (Serial.available()) {
        char c = Serial.read();
        if (isdigit(c)) {
          inputPassword += c;
          showPasswordScreen();
          if (inputPassword.length() == 6) {
            if (inputPassword == correctPassword) {
              openDoor();
              closeDoor();
              break;
            } else {
              attempts++;
              inputPassword = "";
              display.clearDisplay();
              display.setCursor(0, 0);
              display.println("Wrong! Attempts: " + String(attempts));
              display.display();
              delay(1000);
            }
          }
        }
      }
    }

    if (attempts >= 3) {
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("Timeout!!");
      display.display();
      delay(5000);
    }
  }

  // ==== ตรวจจับการออก ====
  if ((digitalRead(BUTTON_PIN) == LOW || (dist2 > 0 && dist2 < 30)) && !doorOpen) {
    openDoor();
    closeDoor();
  }

  delay(100);
}