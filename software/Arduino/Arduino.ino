#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_PWMServoDriver.h>
#include "src/animations/sleep/sleep.h"
#include "src/animations/up/Up.h"
#include "src/animations/walk/walk.h"
//#include "src/animations/walk/walkLeft.h"
#include "src/animations/default/default.h"
#include "src/functions/servo.h"

#include "src/animations/InverseKinematic/rouli/rouli.h"


const char* ssid = "HexaApp";
const char* password = "12345";

AsyncWebServer server(80);

float speed = 2.5;
bool isWalking = false;
unsigned long previousMillis = 0;
int walkCycles = 0;
const int totalWalkCycles = 4; // Nombre total de cycles de marche à exécuter

void startWalking() {
  isWalking = true;
  walkCycles = 0;
  previousMillis = millis();
}

void setup() {
  Serial.begin(115200);
  Init();

  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("Point d'accès IP: ");
  Serial.println(IP);

  Up(3);
  //vitesse, Top, Bottom, Left, Right
  Rouli(4, 3.0, 0.0, 0.0, 0.0);
  Rouli(4, 0.0, 3.0, 0.0, 0.0);
  Rouli(4, 0.0, 0.0, 3.0, 0.0);
  Rouli(4, 0.0, 0.0, 0.0, 3.0);

  Rouli(4, 0.0, 0.0, 0.0, 0.0);
  
  Rouli(4, 3.0, 0.0, 3.0, 0.0);
  Rouli(4, 3.0, 0.0, 0.0, 3.0);
  Rouli(4, 0.0, 3.0, 0.0, 3.0);
  Rouli(4, 0.0, 3.0, 3.0, 0.0);

  Rouli(4, 0.0, 0.0, 0.0, 0.0);

  Sleep(3);
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = R"rawliteral(
      <html>
      <head>
        <title>Contrôle du robot hexapode</title>
        <style>
          body { font-family: Arial, sans-serif; text-align: center; padding-top: 50px; }
          button { padding: 15px 25px; margin: 5px; font-size: 16px; }
          .slider-container { margin-top: 20px; }
          input[type="range"] { width: 80%; }
        </style>
      </head>
      <body>
        <h1>Contrôle du robot hexapode</h1>
        <button onclick="sendRequest('/walk')">Marche</button>
        <button onclick="sendRequest('/up')">Lever</button>
        <button onclick="sendRequest('/sleep')">Dodo</button>
        <button onclick="sendRequest('/default')">Par défaut</button>
        <div class="slider-container">
          <label for="speedRange">Vitesse: <span id="speedValue">2.5</span></label><br>
          <input type="range" id="speedRange" min="0.5" max="5.0" step="0.1" value="2.5" oninput="updateSpeed(this.value)">
        </div>
        <script>
          function sendRequest(action) {
            fetch(action)
              .then(response => response.text())
              .then(data => alert(data))
              .catch(error => console.error('Error:', error));
          }
          
          function updateSpeed(value) {
            document.getElementById('speedValue').innerText = value;
            fetch('/speed?value=' + value)
              .catch(error => console.error('Error:', error));
          }
        </script>
      </body>
      </html>
    )rawliteral";
    request->send(200, "text/html", html);
  });

  server.on("/walk", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Le robot marche!");
    startWalking();
  });

  server.on("/up", HTTP_GET, [](AsyncWebServerRequest *request){
    Up(speed);
    request->send(200, "text/plain", "Le robot se lève!");
  });

  server.on("/sleep", HTTP_GET, [](AsyncWebServerRequest *request){
    Sleep(speed);
    request->send(200, "text/plain", "Le robot dort!");
  });

  server.on("/default", HTTP_GET, [](AsyncWebServerRequest *request){
    Default();
    request->send(200, "text/plain", "Le robot est en position par défaut!");
  });

  server.on("/speed", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("value")) {
      String value = request->getParam("value")->value();
      speed = value.toFloat();
      request->send(200, "text/plain", "Vitesse mise à jour à: " + value);
    } else {
      request->send(400, "text/plain", "Paramètre de vitesse manquant");
    }
  });

  server.begin();
  

  /*
  Default();
  Walk(speed);
  Walk(speed);
  Walk(speed * 2);
  Walk(speed * 2);
  Walk(1);
  Walk(1);
  */
}

void loop() {
  if (isWalking) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 500*speed) {
      previousMillis = currentMillis;
      Walk(speed);
      walkCycles++;
      if (walkCycles >= totalWalkCycles) {
        isWalking = false;
      }
    }
  }
}
