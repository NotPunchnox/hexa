
#include <Adafruit_PWMServoDriver.h>
#include "src/animations/sleep/sleep.h"
#include "src/animations/up/Up.h"
#include "src/animations/walk/walk.h"
#include "src/animations/walk/walkLeft.h"
#include "src/animations/InverseKinematic/rouli/rouli.h"

#include "src/animations/InverseKinematic/poses/Top.h"
#include "src/router/Walking.h"
#include "src/animations/InverseKinematic/rouli/turnZ.h"
#include "src/animations/default/default.h"

#include "src/animations/InverseKinematic/turn/turn.h"

#include "src/AI/MoveLogic.h"

void performAnimations() {
    Up(4);
    TurnZ("horaire", 1.5, 1);
    TurnZ("anti-horaire", 1.3, 1);

    ChangeTop(3, 0);//speed & +cm
    delay(2000);
    ChangeTop(3, 2);//speed & +cm
    delay(2000);
    ChangeTop(3, -2);//speed & +cm
    delay(2000);

    Sleep(3);
}

void setup() {
  Serial.begin(115200);
  Init();

  Up(4);
  //performAnimations();
}

void loop() {

  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');

    String response = "Commands: " + command;
    Serial.println(response);

    IA_Movements(response);
    
  }

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
