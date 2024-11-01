#include "../../config/config.h"
#include "../../functions/Algo.h"
#include "../../functions/servo.h"
#include "../walk/walk.h"
#include <Arduino.h>

void Jump(float speed) {

  float matrice_up[][5] = {
    {PX, PZ/3, PY, 200  * speed, 500},
    {PX, PZ, PY, 50  * speed, 0},
    {PX, PZ/2, PY, 150  * speed, 0},
    {PX, PZ, PY, 300  * speed, 0},
  };

  for (int i = 0; i < sizeof(matrice_up) / sizeof(matrice_up[0]); ++i) {
        int legIndices[] = {0, 1, 2, 3, 4, 5}; // Indices des pattes à déplacer en même temps

        moveLegsSmoothly(legIndices, 6, matrice_up[i][0], matrice_up[i][1], matrice_up[i][2], matrice_up[i][3]);

    delay(matrice_up[i][4]);
  }
  Serial.println("Animation:Jump:terminé");
}
