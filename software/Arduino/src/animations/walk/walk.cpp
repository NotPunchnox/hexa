#include "../../config/config.h"
#include "../../functions/Algo.h"
#include <Arduino.h>
#include "../../functions/servo.h"
#include "../sleep/sleep.h"

// function pour faire aller une patte du robot dans les coordonnées désirés
void setLeg(float x, float z, float y, int duree, int LEG[3], int address) {
  if(LEG == LFL || LEG == LBL) {
    x = x-2;
    y = y+11;
  }
  if(LEG == LFR || LEG == LBR) {
    x=x-2;
    y = y-11;
  }
  LegAngles res = Algo(x, z, y, duree);

  float angles[3];

  if (LEG == LML || LEG == LFL || LEG == LBL) {//Si c'est une patte de gauche alors il faut inverser les angles tibia et femur
    
    angles[0] = res.AngleTibia;
    angles[1] = 180-res.AngleFemur;
    angles[2] = res.AngleCoxa;
  } else {
    angles[0] = 180-res.AngleTibia;
    angles[1] = res.AngleFemur;
    angles[2] = res.AngleCoxa;
  };

  setServo(LEG, 3, angles, address, duree);
}

// R: Rayon cercle virtuel: 5cm ( extension de la patte lors de la marche ).
// Matrice de positions dans un environnement 3D (x, z, y, ms, attente) => hauteur, extension, rotation coxa en cm, temps du mouvement, temps d'attente une fois le mouvement fait.

float R = 2.5;

float WalkUpMatrice[][5] = {
  {PX, PZ, PY, 20, 1000},//Patte par défaut
  {PX, 1, PY, 600, 0},//Patte levé sur l'axe Z
  {PX, 1, 2*R, 200, 0},//Avancer la patte sur l'axe Y
  {PX, PZ, 2*R, 600, 0},//Reposer la patte sur l'axe Z
  {PX, PZ, R, 500, 0},
  {PX, PZ, -R, 500, 2000},

  {PX, PZ-5, -R, 500, 0},
  {PX, PZ, PY, 500, 0},
};
float WalkBottomMatrice[][5] = {
  {PX, PZ, PY, 500, 1000},//Patte par défaut
  {PX, PZ, 2*R, 700, 200},//Patte recule sur l'axe Y
};

void Walk() {
  for (int i = 0; i < sizeof(WalkUpMatrice) / sizeof(WalkUpMatrice[0]); ++i) {

    //LEG FRONT RIGHT
    setLeg(WalkUpMatrice[i][0], WalkUpMatrice[i][1], WalkUpMatrice[i][2], WalkUpMatrice[i][3], LFL, 1);
    //setLeg(WalkUpMatrice[i][0], WalkUpMatrice[i][1], WalkUpMatrice[i][2], WalkUpMatrice[i][3], LMR, 0);
    //setLeg(WalkUpMatrice[i][0], WalkUpMatrice[i][1], WalkUpMatrice[i][2], WalkUpMatrice[i][3], LBL, 1);
    
    //LEG
    if(i ==1 || i==2) {

      //setLeg(WalkBottomMatrice[i-1][0], WalkBottomMatrice[i][1], WalkBottomMatrice[i-1][2], WalkBottomMatrice[i-1][3], LFR, 0);
      //setLeg(WalkBottomMatrice[i-1][0], WalkBottomMatrice[i][1], WalkBottomMatrice[i-1][2], WalkBottomMatrice[i-1][3], LML, 1);
      //setLeg(WalkBottomMatrice[i-1][0], WalkBottomMatrice[i-1][1], -WalkBottomMatrice[i-1][2], WalkBottomMatrice[i-1][3], LBR, 0);

    }
    
    delay(WalkUpMatrice[i][4]);
  }
}
