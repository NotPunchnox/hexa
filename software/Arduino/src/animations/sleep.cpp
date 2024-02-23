#include "../config/config.h"
#include "../functions/Algo.h"
#include "../functions/Servo.h"
#include <Arduino.h>

// Matrice de sleep_animation (x, z, y, ms) => hauteur, extension, rotation coxa en cm, temps du mouvement.
float matrice_sleep[][5] = {
  {9, 10, 0, 0, 1000},
  {9, 8, 0, 0, 250},
  {9, 6, 0, 0, 250},
  {9, 4, 0, 0, 250},
  {9, 2, 0, 0, 250},
  {9, -2, 0, 0, 250},
  {8, -2, 0, 0, 250}
  //{9, 5, 0, 500, 1},
  //{9, 1, 0, 50, 1},
};

void SetLeg(float x, float z, float y, int duree, int* LEG) {
  LegAngles res = Algo(x, z, y, duree);

  float angles[3];

  if (LEG == LFL || LEG == LML) {//Si c'est une patte de gauche alors il faut inverser les angles tibia et femur

    angles[0] = 180 - res.AngleTibia;
    angles[1] = res.AngleFemur;
    angles[2] = res.AngleCoxa;
  } else {
    angles[0] = res.AngleTibia;
    angles[1] = 180 - res.AngleFemur;
    angles[2] = res.AngleCoxa;
  };
  setServo(LEG, 3, angles);
}

void Sleep() {
  for (int i = 0; i < sizeof(matrice_sleep) / sizeof(matrice_sleep[0]); ++i) {
    //Patte avant droit
    SetLeg(matrice_sleep[i][0], matrice_sleep[i][1], matrice_sleep[i][2], matrice_sleep[i][3], LFR);
    //Pattes millieu droite
    SetLeg(matrice_sleep[i][0], matrice_sleep[i][1], matrice_sleep[i][2], matrice_sleep[i][3], LMR);
    //Patte avant gauche
    SetLeg(matrice_sleep[i][0], matrice_sleep[i][1], -matrice_sleep[i][2], matrice_sleep[i][3], LMR);
    //Patte millieu gauche
    SetLeg(matrice_sleep[i][0], matrice_sleep[i][1], -matrice_sleep[i][2], matrice_sleep[i][3], LMR);


    delay(matrice_sleep[i][4]);
  }
}
