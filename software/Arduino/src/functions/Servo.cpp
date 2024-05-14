#include "servo.h"
#include "../config/config.h"
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x41);

#define MIN_PULSE_WIDTH 600
#define MAX_PULSE_WIDTH 2400
#define SERVO_FREQ 50

// Limites des angles des servomoteurs
#define MIN_ANGLE 0
#define MAX_ANGLE 180

void Init() {
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);

  pwm2.begin();
  pwm2.setOscillatorFrequency(27000000);
  pwm2.setPWMFreq(SERVO_FREQ);
}

unsigned int pwmToMicroseconds(unsigned int pwmValue) {
  float frequency = SERVO_FREQ;
  unsigned int cycleDuration = 1000000 / frequency;
  return (pwmValue * cycleDuration) / 4096;
}


// Calcule l'angle intermédiaire entre l'angle actuel et l'angle cible tout en respectant les limites
float interpolateAngle(float currentAngle, float targetAngle, unsigned long elapsedTime, int duree) {
  float angleDiff = targetAngle - currentAngle;
  float angleChange = angleDiff * (float(elapsedTime) / duree);
  float newAngle = currentAngle + angleChange;
  // Limite l'angle dans la plage autorisée
  if (newAngle < MIN_ANGLE) {
    newAngle = MIN_ANGLE;
  } else if (newAngle > MAX_ANGLE) {
    newAngle = MAX_ANGLE;
  }
  return newAngle;
}
int pulseWidth(float angle) {
  int pulseWidthMicros = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  return pulseWidthMicros;
}

// Déplace les servomoteurs vers les angles cibles de manière progressive sur une durée définie
void moveServosSmoothly(int servoChannels[], int numChannels, int duree, float targetAngles[], int address) {
  unsigned long startTime = millis();
  unsigned long currentTime;
  float currentAngles[numChannels];

  // Déplace progressivement les servomoteurs vers les angles cibles
  do {
    currentTime = millis();
    float elapsed = currentTime - startTime;

    for (int i = 0; i < numChannels; ++i) {
      if (address == 1) {
        currentAngles[i] = pwmToMicroseconds(pwm.getPWM(servoChannels[i]));
        float newAngle = interpolateAngle(currentAngles[i], targetAngles[i], elapsed, duree);
        pwm2.writeMicroseconds(servoChannels[i], pulseWidth(newAngle));
      } else {
        currentAngles[i] = pwmToMicroseconds(pwm.getPWM(servoChannels[i]));
        float newAngle = interpolateAngle(currentAngles[i], targetAngles[i], elapsed, duree);
        pwm.writeMicroseconds(servoChannels[i], pulseWidth(newAngle));
      }
    }
    delay(10);
  } while (currentTime - startTime < duree);
}

void setServo(int servoChannels[], int numChannels, float angles[], int address, int duree) {
  moveServosSmoothly(servoChannels, numChannels, duree, angles, address);
}
