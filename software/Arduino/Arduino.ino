#include <Adafruit_PWMServoDriver.h>
#include "src/animations/sleep/sleep.h"
#include "src/animations/up/Up.h"
#include "src/animations/walk/walk.h"
#include "src/animations/InverseKinematic/rouli/rouli.h"

#include "src/animations/InverseKinematic/poses/Top.h"
#include "src/router/Walking.h"
#include "src/animations/InverseKinematic/rouli/turnZ.h"
#include "src/animations/default/default.h"
#include "src/animations/InverseKinematic/turn/turn.h"
#include "src/AI/MoveLogic.h"


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
    if (currentMillis - previousMillis >= 240*speed) {
      previousMillis = currentMillis;
      Walk(speed, X, Y);
    }
  }

}
