#include "../../config/config.h"
#include "../../functions/Algo.h"
#include "../../functions/servo.h"
#include <Arduino.h>

void Default(float speed) {
  int legIndices[] = {0, 1, 2, 3, 4, 5};

  PX = 10;
  PZ = 9;
  PY = 0;

  moveLegsSmoothly(legIndices, 6, PX, PZ, PY, 200*speed);
}