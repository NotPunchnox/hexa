#include <Arduino.h>

float speed = 2.5;
bool isWalking = false;
unsigned long previousMillis = 0;

int walkCycles = 0;
int totalWalkCycles = 4;

void startWalking() {
  isWalking = true;
  walkCycles = 0;
  previousMillis = millis();
}