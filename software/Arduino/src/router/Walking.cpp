#include <Arduino.h>

float speed = 2.5;
bool isWalking = false;
unsigned long previousMillis = 0;

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
}