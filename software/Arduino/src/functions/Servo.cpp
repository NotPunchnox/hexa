#include "servo.h"
#include "../config/config.h"
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

#define SERVOMIN  150
#define SERVOMAX  600
#define SERVO_FREQ 50

void Init() {
  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);
}

void setServo(int servoChannels[], int numChannels, float angles[]) {
    for (int i = 0; i < numChannels; ++i) {
        if (servoChannels[i] >= 0 && servoChannels[i] <= 15) {
            int pulse = map(angles[i], 0, 180, SERVOMIN, SERVOMAX);
            pwm.setPWM(servoChannels[i], 0, pulse);
        } else {
            Serial.println("Erreur : Canal de servo invalide.");
        }
    }
}
