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
void setLegs(LegConfig legs[]);

#endif
