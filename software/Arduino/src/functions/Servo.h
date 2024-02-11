#ifndef SERVO_H
#define SERVO_H

#include <Adafruit_PWMServoDriver.h>

void setServo(int servoChannels[], int numChannels, float angles[]);
void Init();

#endif
