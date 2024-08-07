#ifndef SERVO_H
#define SERVO_H

#include <Adafruit_PWMServoDriver.h>

struct LegConfig {
  int servoChannels[3];
  float coxa;
  float femur;
  float tibia;
  int address;
  int duree;
};

void setServo(int servoChannels[], int numChannels, float angles[], int address, int duree);
void Init();
void setLeg(float x, float z, float y, int duree, int LEG[3], int address);
void moveLegsSmoothly(int legIndices[], int numLegs, float targetX, float targetZ, float targetY, int duration);
void moveLegsMatrices(int legIndices[], float targetX[], float targetZ[], float targetY[], int numLegs, int duration);

#endif
