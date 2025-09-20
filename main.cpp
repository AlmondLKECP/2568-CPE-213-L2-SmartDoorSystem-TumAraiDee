#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Servo.h>

HardwareSerial mySerial(2);


// ==== CONFIG ==== 
#define SERVO_PIN 14
#define TRIG_PIN1 32
#define ECHO_PIN1 33
#define TRIG_PIN2 25
#define ECHO_PIN2 26
#define BUTTON_PIN 27   // ปุ่มออก
#define LED1 18
#define LED2 19

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Servo doorServo;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ==== PASSWORD ==== 
String correctPassword = "256574"; // รหัสผ่านที่ถูกต้อง
String inputPassword = ""; // เก็บรหัสผ่านที่ป้อน
int attempts = 0; // จำนวนความพยายาม

// ==== STATE ==== 
bool doorOpen = false;
bool readyToExit = false;

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

void idleStatus(){
  doorOpen = false;
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Smart Door Ready");
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, HIGH);
  display.display();
}

void openDoor() {
  doorServo.write(90);   // เปิด 90 องศา
  doorOpen = true;
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Door Opened (90)");
  display.display();
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  delay(3000); // เวลาผ่านประตู
}

void closeDoor() {
  doorServo.write(0);   // ปิดประตู
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
  mySerial.begin(115200, SERIAL_8N1, 16, 17);
  Serial.begin(115200);
  mySerial.println("Ready to type...");
  pinMode(TRIG_PIN1, OUTPUT);
  pinMode(ECHO_PIN1, INPUT);
  pinMode(TRIG_PIN2, OUTPUT);
  pinMode(ECHO_PIN2, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  doorServo.attach(SERVO_PIN);
  doorServo.write(0); // ปิดเริ่มต้น

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    mySerial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Smart Door Ready");
  display.display();
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, HIGH);
}

bool waitingForPassword = false;

void loop() {
  long dist1 = readUltrasonic(TRIG_PIN1, ECHO_PIN1);
  long dist2 = readUltrasonic(TRIG_PIN2, ECHO_PIN2);

  // ==== ตรวจจับเข้าด้านหน้า (เริ่มให้ใส่รหัส) ====
  if (dist1 > 0 && dist1 < 30 && !doorOpen && !waitingForPassword) {
    inputPassword = "";
    attempts = 0;
    waitingForPassword = true;
    showPasswordScreen();
  }

  // ==== อ่านรหัสผ่านจาก Serial แบบ non-blocking ====
  if (waitingForPassword) {
    if (mySerial.available()) {
      char c = mySerial.read();
      if (isdigit(c)) {
        inputPassword += c;
        showPasswordScreen();

        if (inputPassword.length() == 6) {
          if (inputPassword == correctPassword) {
            openDoor();
            closeDoor();
            idleStatus();
            waitingForPassword = false;
          } else {
            attempts++;
            inputPassword = "";
            display.clearDisplay();
            display.setCursor(0, 0);
            display.println("Wrong! Attempts: " + String(attempts));
            display.display();
            delay(1000);
            if (attempts >= 3) {
              display.clearDisplay();
              display.setCursor(0, 0);
              display.println("Timeout!!");
              display.display();
              digitalWrite(LED1, HIGH);
              digitalWrite(LED2, LOW);
              delay(5000);
              idleStatus();
              waitingForPassword = false;
            } else {
              showPasswordScreen();
            }
          }
        }
      }
    }
  }

  // ==== ตรวจจับการออก ====
  if(dist2 > 0 && dist2 < 30 && !doorOpen){
    readyToExit = true;
  }
  if(readyToExit && digitalRead(BUTTON_PIN)==LOW && !doorOpen){
    openDoor();
    closeDoor();
    idleStatus();
    readyToExit = false;
  }

  delay(50);
}