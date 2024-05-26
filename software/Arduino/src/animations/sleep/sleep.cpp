#include "../../config/config.h"
#include "../../functions/Algo.h"
#include "../../functions/servo.h"
#include <Arduino.h>

// Matrice de sleep_animation (x, z, y, ms) => hauteur, extension, rotation coxa en cm, temps du mouvement.
float matrice_sleep[][5] = {
  {PX, PZ, PY, 0, 1000},
  {PX, PZ-(PZ/4), PY, 0, 500},
  {PX, PZ/2, PY, 0, 500},
  {PX, PZ/4, PY, 0, 500},
  {PX, 1, PY, 0, 1000},
};


void Sleep(float speed) {
  float matrice_sleep[][5] = {
    {PX, PZ, PY, 200  * speed, 20},
    {PX, PZ-(PZ/4), PY, 300  * speed, 0},
    {PX, PZ/2, PY, 300  * speed, 0},
    {PX, PZ/4, PY, 300  * speed, 0},
    {PX, 1, PY, 300  * speed, 1000},
  };

  for (int i = 0; i < sizeof(matrice_sleep) / sizeof(matrice_sleep[0]); ++i) {
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

        moveLegsSmoothly(legIndices, 6, matrice_sleep[i][0], matrice_sleep[i][1], matrice_sleep[i][2], matrice_sleep[i][3]);

    delay(matrice_sleep[i][4]);
  }
}