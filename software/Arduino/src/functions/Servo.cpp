#include "servo.h"
#include "../config/config.h"
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x41);

#define MIN_PULSE_WIDTH 600
#define MAX_PULSE_WIDTH 2600
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
    int pulse_wide, analog_value;
    pulse_wide = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
    analog_value = int(float(pulse_wide) / 1000000 * SERVO_FREQ * 4096);
    return analog_value;
}

void setServo(int servoChannels[], int numChannels, float angles[], int address) {
    for (int i = 0; i < numChannels; ++i) {
        if (address == 1) {
            pwm2.setPWM(servoChannels[i], 0, pulseWidth(angles[i]));
        } else {
            pwm.setPWM(servoChannels[i], 0, pulseWidth(angles[i]));
        }
    }
}
