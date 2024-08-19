#include <Arduino.h>

float speed = 2.5;
bool isWalking = false;
unsigned long previousMillis = 0;

float X = 0;
float Y = 0;

int walkCycles = 0;
int totalWalkCycles = 4;

void startWalking(float x, float y, float newspeed) {
  X = x;
  Y = y;
  speed = newspeed;
  isWalking = true;
  walkCycles = 0;
  previousMillis = millis();
}