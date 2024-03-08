#include "Wire.h"
#include "Adafruit_PWMServoDriver.h"
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x40);

#define MIN_PULSE_WIDTH 600
#define MAX_PULSE_WIDTH 2600
#define FREQUENCY 50

void setup() {
  Serial.begin(9600); // Démarre la communication série
  Serial.println("Initialisation...");

  pwm2.begin();
  pwm2.setPWMFreq(FREQUENCY);
  Serial.println("Fréquence de l'oscillateur : " + String(pwm2.getOscillatorFrequency()));

  // Trouver MIN_PULSE_WIDTH : définir l'angle à 0
  // Trouver MAX_PULSE_WIDTH : définir l'angle à 180
  pwm2.setPWM(0, 0, pulseWidth(90)); // 0 ou 180
  Serial.println('Servo: 0');
  pwm2.setPWM(1, 0, pulseWidth(90)); // 0 ou 180
  Serial.println('Servo: 1');
  pwm2.setPWM(2, 0, pulseWidth(90)); // 0 ou 180
  Serial.println('Servo: 2');
}

int pulseWidth(int angle) {
  int pulse_wide, analog_value;
  pulse_wide = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  return analog_value;
}

void loop() {
}
