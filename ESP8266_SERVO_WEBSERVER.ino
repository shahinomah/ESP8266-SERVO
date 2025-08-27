#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Servo.h>

// Wi-Fi credentials (AP mode)
const char* ssid = "ESP_SERVO";     
const char* password = "12345678";  

// Create AsyncWebServer
AsyncWebServer server(80);

// Servo setup
Servo myservo; 
int servoPos = 90;  // current servo position

// Smooth servo movement
void moveServoSmooth(int targetPos, int stepDelay = 10) {
  if (targetPos > 180) targetPos = 180;
  if (targetPos < 0) targetPos = 0;

  if (servoPos < targetPos) {
    for (int pos = servoPos; pos <= targetPos; pos++) {
      myservo.write(pos);
      delay(stepDelay);
    }
  } else {
    for (int pos = servoPos; pos >= targetPos; pos--) {
      myservo.write(pos);
      delay(stepDelay);
    }
  }
  servoPos = targetPos;
}

void setup() {
  Serial.begin(115200);

  // Start Wi-Fi in AP mode
  WiFi.softAP(ssid, password);
  Serial.println("ESP8266 AP Mode: ESP_SERVO");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  // Attach single servo to D0
  myservo.attach(D0, 500, 2400);
  myservo.write(servoPos);

  // Endpoint to move servo
  server.on("/move", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("pos")) {
      int targetPos = request->getParam("pos")->value().toInt();
      Serial.printf("Servo moving to %d\n", targetPos);
      moveServoSmooth(targetPos);
      request->send(200, "text/plain", "Servo moved");
    } else {
      request->send(400, "text/plain", "Missing parameter");
    }
  });

  // Reset endpoint
  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request){
    moveServoSmooth(90);
    request->send(200, "text/plain", "Servo reset to 90");
  });

  // Web interface with slider
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String page = "<html><body><h1>ESP8266 Servo Control</h1>";
    page += "Servo: <input type='range' min='0' max='180' value='" + String(servoPos) + "' id='s1' ";
    page += "oninput='moveServo(this.value)'><br>";
    page += "<button onclick='resetServo()'>Reset</button>";
    page += "<script>"
            "function moveServo(pos){fetch('/move?pos='+pos);}"
            "function resetServo(){fetch('/reset');document.getElementById('s1').value=90;}"
            "</script></body></html>";
    request->send(200, "text/html", page);
  });

  // Start server
  server.begin();
}

void loop() {
  // Nothing here, AsyncWebServer handles requests
}
