#include "../../config/config.h"
#include "../../functions/Algo.h"
#include "../../functions/Servo.h"
#include "../sleep/sleep.h"
#include <Arduino.h>

// Matrice de sleep_animation (x, z, y, ms) => hauteur, extension, rotation coxa en cm, temps du mouvement.
float matrice_up[][5] = {
  {PX, 1, PY, 0, 1000},
  {PX, PZ/2, PY, 0, 250},
  {PX, PZ, PY, 0, 350},
  {PX, PZ+3, PY, 0, 350},
  /*
  {PX, -2, PY, 0, 1000},
  {PX, -6, PY, 0, 1000},
  */
};

void Up() {
  for (int i = 0; i < sizeof(matrice_up) / sizeof(matrice_up[0]); ++i) {
    /*
    Address = 1 ( 0x41 ):
      LBR
      LBL
      LML
    */

    //Patte avant droite
    SetLeg(matrice_up[i][0], matrice_up[i][1], matrice_up[i][2], matrice_up[i][3], LFR, 0);
    //Pattes millieu droite
    SetLeg(matrice_up[i][0], matrice_up[i][1], matrice_up[i][2], matrice_up[i][3], LMR, 0);
    //Patte avant gauche
    SetLeg(matrice_up[i][0], matrice_up[i][1], matrice_up[i][2], matrice_up[i][3], LBR, 0);
    //Patte millieu gauche
    SetLeg(matrice_up[i][0], matrice_up[i][1], -matrice_up[i][2], matrice_up[i][3], LML, 1);
    //Patte arrière droite
    SetLeg(matrice_up[i][0], matrice_up[i][1], -matrice_up[i][2], matrice_up[i][3], LFL, 1);
    //Patte arrière gauche
    SetLeg(matrice_up[i][0], matrice_up[i][1], -matrice_up[i][2], matrice_up[i][3], LBL, 1);


    delay(matrice_up[i][4]);
  }
}
