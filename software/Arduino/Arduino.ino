
#include <Adafruit_PWMServoDriver.h>
#include "src/animations/sleep/sleep.h"
#include "src/animations/up/Up.h"
#include "src/animations/walk/walk.h"
#include "src/animations/walk/walkLeft.h"
#include "src/animations/InverseKinematic/rouli/rouli.h"

#include "src/animations/InverseKinematic/poses/Top.h"
#include "src/router/Walking.h"

void performAnimations() {
    Up(4);
    TurnZ("horaire", 2);
    TurnZ("anti-horaire", 2);

    delay(2000);

    ChangeTop(2.5, 2);//speed & +cm

    WalkLeft(4);
    Sleep(3);
}

void setup() {
  Serial.begin(115200);
  Init();

  performAnimations();
}

void loop() {


  if (isWalking) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 500*speed) {
      previousMillis = currentMillis;
      Walk(speed);
      walkCycles++;
      if (walkCycles >= totalWalkCycles) {
        isWalking = false;
      }
    }
  }

}
