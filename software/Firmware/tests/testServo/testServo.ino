#include <Wire.h>
#include <ESP8266WiFi.h>
#include <Adafruit_PWMServoDriver.h>

#define PCA9685_ADDR 0x40
#define MIN_PULSE_WIDTH 600
#define MAX_PULSE_WIDTH 2600
#define FREQUENCY 50

Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(PCA9685_ADDR);

void setup() {
  Serial.begin(9600, SERIAL_8N1); // Démarre la communication série
  Serial.println("Initialisation...");

  Wire.begin();
  pwm2.begin();
  pwm2.setPWMFreq(FREQUENCY);
  Serial.print("Fréquence de l'oscillateur : ");
  Serial.println(pwm2.getOscillatorFrequency());

  // Réglez MIN_PULSE_WIDTH : définissez l'angle à 0
  // Réglez MAX_PULSE_WIDTH : définissez l'angle à 180
  // pwm2.setPWM(5, 0, pulseWidth(90)); // 0 ou 180
  // Serial.println('Servo: 0');
  pwm2.setPWM(14, 0, pulseWidth(90)); // 0 ou 180
  Serial.println("Servo: 1");
  // pwm2.setPWM(7, 0, pulseWidth(90)); // 0 ou 180
  // Serial.println('Servo: 2');
}

int pulseWidth(int angle) {
  int pulse_wide, analog_value;
  pulse_wide = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000.0 * FREQUENCY * 4096);
  return analog_value;
}

void loop() {
}
