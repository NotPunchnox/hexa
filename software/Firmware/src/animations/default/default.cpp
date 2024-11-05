#include "../../config/config.h"
#include "../../functions/Algo.h"
#include "../../functions/servo.h"
#include <Arduino.h>

void Default(float speed) {
  int legIndices[] = {0, 1, 2, 3, 4, 5};

  PX = 10;
  PZ = 9;
  PY = 0;
  TARGET_Z[0] = PZ;
  TARGET_Z[1] = PZ;
  TARGET_Z[2] = PZ;
  TARGET_Z[3] = PZ;
  TARGET_Z[4] = PZ;
  TARGET_Z[5] = PZ;

  moveLegsSmoothly(legIndices, 6, PX, PZ, PY, 200*speed);
}