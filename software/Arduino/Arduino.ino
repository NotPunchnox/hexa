//#include "src/animations/sleep/sleep.h"
#include "src/animations/up/Up.h"
#include "src/animations/walk/walk.h"
#include "src/animations/default/default.h"
#include <Adafruit_PWMServoDriver.h>
#include "src/functions/servo.h"

float speed = 2.5;

void setup() {
  Serial.begin(115200);
  Init();

  //delay(1000);
  //Sleep();
  //Up();
  Default();
  Walk(speed);
  Walk(speed);
  Walk(speed*2);
  Walk(speed*2);
  Walk(1);
  Walk(1);
}

void loop() {
  // Boucle vide, le programme ne s'exécute que dans la fonction setup()
}
