#include "src/animations/sleep.h"
#include "src/animations/walk.h"
#include <Adafruit_PWMServoDriver.h>
#include "src/functions/Servo.h"

void setup() {
  Serial.begin(9600);
  Init();

  //Sleep();
  Walk();
}

void loop() {
  // Boucle vide, le programme ne s'exécute que dans la fonction setup()
}
