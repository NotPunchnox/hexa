#include "../../config/config.h"
#include "../../functions/Algo.h"
#include "../../functions/Servo.h"
#include <Arduino.h>

// Matrice de sleep_animation (x, z, y, ms) => hauteur, extension, rotation coxa en cm, temps du mouvement.
float matrice_sleep[][5] = {
  {PX, PZ, PY, 0, 1000},
  {PX, PZ-(PZ/4), PY, 0, 500},
  {PX, PZ/2, PY, 0, 500},
  {PX, PZ/4, PY, 0, 500},
  {PX, 1, PY, 0, 1000},
  /*
  {PX, -2, PY, 0, 1000},
  {PX, -6, PY, 0, 1000},
  */
};

void SetLeg(float x, float z, float y, int duree, int LEG[3], int address) {
  LegAngles res = Algo(x, z, y, duree);

  float angles[3];

  if (LEG == LML || LEG == LFL || LEG == LBL) {//Si c'est une patte de gauche alors il faut inverser les angles tibia et femur
    
    angles[0] = 180-res.AngleTibia;
    angles[1] = res.AngleFemur;
    angles[2] = res.AngleCoxa;
    /*
    if(z < 0) {
      angles[1] = 180 + ;
    } else {
      angles[1] = res.AngleFemur;
    };
    */
  } else {
    angles[0] = res.AngleTibia;
    angles[1] = 180-res.AngleFemur;
    angles[2] = res.AngleCoxa;
  };

  setServo(LEG, 3, angles, address);
  //setServo(LEG, 3, angles, address);
}

void Sleep() {
  for (int i = 0; i < sizeof(matrice_sleep) / sizeof(matrice_sleep[0]); ++i) {
    /*
    Address = 1 ( 0x41 ):
      LBR
      LBL
      LML
    */

    //Patte avant droite
    SetLeg(matrice_sleep[i][0], matrice_sleep[i][1], matrice_sleep[i][2], matrice_sleep[i][3], LFR, 0);
    //Pattes millieu droite
    SetLeg(matrice_sleep[i][0], matrice_sleep[i][1], matrice_sleep[i][2], matrice_sleep[i][3], LMR, 0);
    //Patte avant gauche
    SetLeg(matrice_sleep[i][0], matrice_sleep[i][1], matrice_sleep[i][2], matrice_sleep[i][3], LBR, 0);
    //Patte millieu gauche
    SetLeg(matrice_sleep[i][0], matrice_sleep[i][1], -matrice_sleep[i][2], matrice_sleep[i][3], LML, 1);
    //Patte arrière droite
    SetLeg(matrice_sleep[i][0], matrice_sleep[i][1], -matrice_sleep[i][2], matrice_sleep[i][3], LFL, 1);
    //Patte arrière gauche
    SetLeg(matrice_sleep[i][0], matrice_sleep[i][1], -matrice_sleep[i][2], matrice_sleep[i][3], LBL, 1);


    delay(matrice_sleep[i][4]);
  }
}
