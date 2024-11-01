#include <Arduino.h>

float speed = 2.5;
float rayon = 0;
bool isWalking = false;
bool isTurning = false;
unsigned long previousMillis = 0;
String Side = "";

float X = 0;
float Y = 0;


void startWalking(float x, float y, float newspeed) {
  X = x;
  Y = y;
  speed = newspeed;
  if(!isWalking) {
    isWalking = true;
  }
  if(previousMillis == 0) {
    previousMillis = millis();
  }
  Serial.println("Animation:startWalking:commencé");
}

void startTurning(String s, float newspeed, float r) {
  rayon = r;
  Side = s;
  speed = newspeed;
  if(!isTurning) {
    isTurning = true;
  }
  if(previousMillis == 0) {
    previousMillis = millis();
  }
  Serial.println("Animation:startTurning:commencé");
}