#include "../../config/config.h"
#include "../../functions/Algo.h"
#include <Arduino.h>
#include "../../functions/Servo.h"
#include "../sleep/sleep.h"

// function pour faire aller une patte du robot dans les coordonnées désirés
void setLeg(float x, float z, float y, int duree, int LEG[3], int address) {
  LegAngles res = Algo(x, z, y, duree);

  float angles[3];

  if (LEG == LML || LEG == LFL || LEG == LBL) {//Si c'est une patte de gauche alors il faut inverser les angles tibia et femur
    
    angles[0] = res.AngleTibia;
    angles[1] = 180-res.AngleFemur;
    angles[2] = res.AngleCoxa - 45;//ou + 45 ce sera à jour demain
  } else {
    angles[0] = 180-res.AngleTibia;
    angles[1] = res.AngleFemur;
    angles[2] = res.AngleCoxa - 45;
  };

  setServo(LEG, 3, angles, address);
}

// R: Rayon cercle virtuel: 5cm ( extension de la patte lors de la marche ).
// Matrice de positions dans un environnement 3D (x, z, y, ms, attente) => hauteur, extension, rotation coxa en cm, temps du mouvement, temps d'attente une fois le mouvement fait.

float R = 5;

float WalkUpMatrice[][5] = {
  {PX, PZ, PY, 0, 1000},//Patte par défaut
  {PX, 1, R, 0, 300},//Patte levé sur l'axe Z
  {PX, 1, 2*R, 0, 800},//Avancer la patte sur l'axe Y
  {PX, PZ, 2*R, 0, 300},//Reposer la patte sur l'axe Z
  {PX, PZ, R, PY, 450},//Patte par défaut
};
float WalkBottomMatrice[][5] = {
  {PX, PZ, R, 0, 500},//Patte par défaut
  {PX, PZ, 1, 0, 450},//Patte recule sur l'axe Y
  {PX, PZ, R, 0, 150},//Patte se remet en position défaut
};

void Walk() {
  for (int i = 0; i < sizeof(WalkUpMatrice) / sizeof(WalkUpMatrice[0]); ++i) {

    //LEG FRONT RIGHT
    setLeg(WalkUpMatrice[i][0], WalkUpMatrice[i][1], -WalkUpMatrice[i][2], WalkUpMatrice[i][3], LFR, 0);
    setLeg(WalkUpMatrice[i][0], WalkUpMatrice[i][1], WalkUpMatrice[i][2], WalkUpMatrice[i][3], LML, 1);
    setLeg(WalkUpMatrice[i][0], WalkUpMatrice[i][1], -WalkUpMatrice[i][2], WalkUpMatrice[i][3], LBR, 0);
    /*
    //LEG
    if(i == 2 || i == 3) {

      setLeg(WalkBottomMatrice[i-1][0], WalkBottomMatrice[i-1][1], -WalkBottomMatrice[i-1][2], WalkBottomMatrice[i-1][3], LFL, 1);
      setLeg(WalkBottomMatrice[i-1][0], WalkBottomMatrice[i-1][1], WalkBottomMatrice[i-1][2], WalkBottomMatrice[i-1][3], LMR, 0);
      setLeg(WalkBottomMatrice[i-1][0], WalkBottomMatrice[i-1][1], -WalkBottomMatrice[i-1][2], WalkBottomMatrice[i-1][3], LBL, 1);

    }
    */
    delay(WalkUpMatrice[i][4]);
  }
}
