#include "src/animations/sleep/sleep.h"
#include "src/animations/up/Up.h"
#include "src/animations/walk/walk.h"

#include <Adafruit_PWMServoDriver.h>
#include "src/functions/Servo.h"

void setup() {
  Serial.begin(9600);
  Init();

  Up();
  delay(1000);
  Sleep();
}

void loop() {
  // Boucle vide, le programme ne s'exécute que dans la fonction setup()
}
