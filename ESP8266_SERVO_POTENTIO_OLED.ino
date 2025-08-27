#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display size
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Servo and potentiometer
Servo myservo;
int potPin = A0;   // Potentiometer connected to A0
int potValue = 0;  // Variable to store potentiometer value
int angle = 0;     // Servo angle

void setup() {
  // Initialize servo
  myservo.attach(D0, 500, 2400);  // Servo on pin D5 (GPIO14) for ESP8266

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for most OLEDs
    for (;;); // Don’t proceed, loop forever if OLED not found
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Ready");
  display.display();
  delay(1000);
}

void loop() {
  // Read potentiometer value (0 - 1023)
  potValue = analogRead(potPin);

  // Map potentiometer value to servo angle (0 - 180 degrees)
  angle = map(potValue, 0, 1023, 0, 180);

  // Move servo
  myservo.write(angle);

  // Show angle on OLED
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("Angle:");
  display.setTextSize(3);
  display.setCursor(20, 30);
  display.print(angle);
  display.print((char)247); // degree symbol
  display.display();

  delay(50);
}
