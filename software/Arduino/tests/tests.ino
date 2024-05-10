#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define SERVOMIN  150
#define SERVOMAX  600
#define SERVO_FREQ   50

void setup() {
  Serial.begin(9600);

  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);

}

void loop() {
  // Boucle vide, le programme ne s'exécute que dans la fonction setup()
}