#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>

// OLED setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Servo setup
Servo myservo;
int potPin = A0;   // Potentiometer connected to A0
int potValue = 0;
int angle = 0;

// LED pins
int ledPins[] = {D3, D4, D5, D6, D7, D8};  // 6 LEDs for 30,60,90,120,150,180
int thresholds[] = {30, 60, 90, 120, 150, 180};

void setup() {
  // Servo attach
  myservo.attach(D0, 500, 2400);  // Servo on D0 (GPIO16)

  // LED pins as output
  for (int i = 0; i < 6; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  // Initialize OLED
  Wire.begin(D2, D1); // SDA=D2(GPIO4), SCL=D1(GPIO5)
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;); // Stop if OLED not found
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  // Read potentiometer value
  potValue = analogRead(potPin);
  angle = map(potValue, 0, 1023, 0, 180);

  // Move servo
  myservo.write(angle);

  // Control LEDs
  for (int i = 0; i < 6; i++) {
    if (angle >= thresholds[i]) {
      digitalWrite(ledPins[i], HIGH);
    } else {
      digitalWrite(ledPins[i], LOW);
    }
  }

  // Display servo angle only
  display.clearDisplay();
  display.setCursor(0, 20);
  display.print("Angle:");
  display.print(angle);
  display.println((char)247); // degree symbol
  display.display();

  delay(50);
}
