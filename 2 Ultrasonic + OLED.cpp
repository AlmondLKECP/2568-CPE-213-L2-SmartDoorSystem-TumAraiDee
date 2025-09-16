#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ===== OLED Config =====
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ===== Ultrasonic Pins =====
#define TRIG_PIN1 32
#define ECHO_PIN1 33

#define TRIG_PIN2 25
#define ECHO_PIN2 26

// ---- Ultrasonic ----
float measureOnceUS(int trigPin, int echoPin, uint32_t timeout_us = 30000) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long duration = pulseIn(echoPin, HIGH, timeout_us);
  return (float)duration;
}

float usToCm(float us) {
  return us <= 0 ? NAN : (us * 34 / 2000.0f);
}

float median5cm(int trigPin, int echoPin) {
  float vals[5];
  for (int i = 0; i < 5; i++) {
    vals[i] = usToCm(measureOnceUS(trigPin, echoPin));
    delay(60);
  }
  // Sort
  for (int i = 0; i < 5; i++)
    for (int j = i + 1; j < 5; j++)
      if (vals[j] < vals[i]) {
        float t = vals[i];
        vals[i] = vals[j];
        vals[j] = t;
      }
  return vals[2]; // median
}

// ---- Setup ----
void setup() {
  Serial.begin(9600);

  pinMode(TRIG_PIN1, OUTPUT);
  pinMode(ECHO_PIN1, INPUT);
  pinMode(TRIG_PIN2, OUTPUT);
  pinMode(ECHO_PIN2, INPUT);

  // OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 failed");
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("System ready");
  display.display();
  delay(1000);
}

// ---- Loop ----
void loop() {
  float d1 = median5cm(TRIG_PIN1, ECHO_PIN1);
  float d2 = median5cm(TRIG_PIN2, ECHO_PIN2);

  // Serial Output
  Serial.printf("US1: %.1f cm, US2: %.1f cm\n", d1, d2);

  // OLED Display
  display.clearDisplay();
  display.setCursor(0, 0);
  display.printf("US1 Distance: %.1f cm\n", d1);
  display.printf("US2 Distance: %.1f cm\n", d2);
  display.display();

  delay(200);
}
