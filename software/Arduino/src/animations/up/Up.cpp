#include "../../config/config.h"
#include "../../functions/Algo.h"
#include "../../functions/servo.h"
#include <Arduino.h>

void Up(float speed) {

  float matrice_up[][5] = {
    {PX, 1, PY, 200  * speed, 500},
    {PX, PZ/2, PY, 150  * speed, 0},
    {PX, PZ, PY, 250  * speed, 0},
    {PX, PZ+3, PY, 250  * speed, 0},
  };

  for (int i = 0; i < sizeof(matrice_up) / sizeof(matrice_up[0]); ++i) {
         /*
        {
          0: LFL -> leg front left;
          1: LBL -> leg back left;
          2: LML -> leg middle left;
          3: LFR -> leg front right;
          4: LMR -> leg middle right;
          5: LBR -> leg back right;
        }
        */
        int legIndices[] = {0, 1, 2, 3, 4, 5}; // Indices des pattes à déplacer en même temps

        moveLegsSmoothly(legIndices, 6, matrice_up[i][0], matrice_up[i][1], matrice_up[i][2], matrice_up[i][3]);

    delay(matrice_up[i][4]);
  }
}
