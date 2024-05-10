#include "servo.h"
#include "../config/config.h"
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x41);

#define MIN_PULSE_WIDTH 600
#define MAX_PULSE_WIDTH 2400
#define SERVO_FREQ 50

void Init() {
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);

  pwm2.begin();
  pwm2.setOscillatorFrequency(27000000);
  pwm2.setPWMFreq(SERVO_FREQ);
}

int pulseWidth(int angle) {
  int pulseWidthMicros = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  Serial.println(int(pulseWidthMicros));
  return int(pulseWidthMicros);
}

void setServo(int servoChannels[], int numChannels, float angles[], int address) {
  for (int i = 0; i < numChannels; ++i) {
    if (address == 1) {
      pwm2.writeMicroseconds(servoChannels[i], pulseWidth(angles[i]));
    } else {
      pwm.writeMicroseconds(servoChannels[i], pulseWidth(angles[i]));
    }
  }
}