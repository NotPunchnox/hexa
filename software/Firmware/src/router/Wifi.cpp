#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include "../animations/sleep/sleep.h"
#include "../animations/up/Up.h"
#include "../animations/walk/walk.h"
#include "../animations/default/default.h"
#include "./Walking.h"

const char* ssid = "HexaApp";
const char* password = "12345";

AsyncWebServer server(80);

void StartRouter() {
    WiFi.softAP(ssid, password);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("Point d'accès IP: ");
    Serial.println(IP);


    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = R"rawliteral(
        <html>
        <head>
            <title>Aash controller</title>
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

    server.begin();
}