#include "../config/config.h"
#include "../functions/Algo.h"
#include <Arduino.h>

// Matrice de sleep_animation (x, z, y, ms) => hauteur, extension, rotation coxa en cm, temps du mouvement.
float sleep_animation[][5] = {
  {9, 10, 0, 500, 1000},
  {9, 5, 0, 500, 1},
  {9, 1, 0, 50, 1},
  {9, -3, 0, 500, 1},
  {9, -3, 0, 1000, 1000},
  {9, -5, 0, 1000, 1000},
};

void Sleep() {
    for (int i = 0; i < sizeof(sleep_animation) / sizeof(sleep_animation[0]); ++i) {
    Algo(sleep_animation[i][0], sleep_animation[i][1], sleep_animation[i][2], sleep_animation[i][3]);
    delay(sleep_animation[i][4]);
  }
}
