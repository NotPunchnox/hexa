#include "servo.h"
#include "../config/config.h"
#include "../config/Angles.h"
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x41);

#define MIN_PULSE_WIDTH 600
#define MAX_PULSE_WIDTH 2400
#define SERVO_FREQ 50

// Limites des angles des servomoteurs
#define MIN_ANGLE 0
#define MAX_ANGLE 180
#define MAX_LEG_PARTS 3

void Init() {
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);

  pwm2.begin();
  pwm2.setOscillatorFrequency(27000000);
  pwm2.setPWMFreq(SERVO_FREQ);
}

int pulseWidth(float angle) {
  int pulseWidthMicros = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  return pulseWidthMicros;
}

void moveServosSmoothly(int servoChannels[], int numChannels, int duration, float targetAngles[], int address) {
  unsigned long startTime = millis(); // Heure de début
  int leg = servoChannels[0];
  float legStartAngles[numChannels] = {startAngles[leg].coxa, startAngles[leg].femur, startAngles[leg].tibia};
  float deltaAngles[numChannels];
  float newAngles[numChannels];

  for (int i = 0; i < numChannels; ++i) {
      deltaAngles[i] = targetAngles[i] - legStartAngles[i];
  }

  while (millis() - startTime < duration) { // Tant que le temps écoulé est inférieur à la durée
      unsigned long currentTime = millis();
      float progress = (float)(currentTime - startTime) / duration;

      for (int i = 0; i < numChannels; ++i) {
          newAngles[i] = legStartAngles[i] + deltaAngles[i] * progress;
      }

      for (int i = 0; i < numChannels; ++i) {
          if (address == 1) {
              pwm2.writeMicroseconds(servoChannels[i], pulseWidth(newAngles[i]));
          } else {
              pwm.writeMicroseconds(servoChannels[i], pulseWidth(newAngles[i]));
          }
      }
  }

  for (int i = 0; i < numChannels; ++i) {
      startAngles[leg].coxa = newAngles[0];
      startAngles[leg].femur = newAngles[1];
      startAngles[leg].tibia = newAngles[2];
  }
  Serial.println(startAngles[leg].coxa);
  Serial.println(startAngles[leg].femur);
  Serial.println(startAngles[leg].tibia);
}

void setServo(int servoChannels[], int numChannels, float angles[], int address, int duree) {
  /*if(startAngles[servoChannels[0]].femur > 0) {
   moveServosSmoothly(servoChannels, numChannels, duree, angles, address);
  } else {*/
    for (int i = 0; i < numChannels; ++i) {
        if (address == 1) {
          pwm2.writeMicroseconds(servoChannels[i], pulseWidth(angles[i]));
        } else {
          pwm.writeMicroseconds(servoChannels[i], pulseWidth(angles[i]));
        }
    }
    for (int i = 0; i < numChannels; ++i) {
      startAngles[servoChannels[0]].coxa = angles[0];
      startAngles[servoChannels[0]].femur = angles[1];
      startAngles[servoChannels[0]].tibia = angles[2];
    }
//}
  
}