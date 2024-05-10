#include "../../config/config.h"
#include "../../functions/Algo.h"
#include <Arduino.h>
#include "../../functions/Servo.h"
#include "../sleep/sleep.h"


// R: Rayon cercle virtuel: 5cm ( extension de la patte lors de la marche ).
// Matrice de positions dans un environnement 3D (x, z, y, ms, attente) => hauteur, extension, rotation coxa en cm, temps du mouvement, temps d'attente une fois le mouvement fait.

float R = 5;

float WalkUpMatrice[][5] = {
  {PX, PZ, R, 0, 500},//Patte par défaut
  {PX, 1, R, 0, 100},//Patte levé sur l'axe Z
  {PX, 1, 2*R, 0, 350},//Avancer la patte sur l'axe Y
  {PX, PZ, 2*R, 0, 100},//Reposer la patte sur l'axe Z
  {PX, PZ, R, 0, 150},//Patte par défaut
};
float WalkBottomMatrice[][5] = {
  {PX, PZ, R, 0, 500},//Patte par défaut
  {PX, PZ, 1, 0, 450},//Patte recule sur l'axe Y
  {PX, PZ, R, 0, 150},//Patte se remet en position défaut
};

void Walk() {
  for (int i = 0; i < sizeof(WalkUpMatrice) / sizeof(WalkUpMatrice[0]); ++i) {

    //LEG FRONT RIGHT
    SetLeg(WalkUpMatrice[i][0], WalkUpMatrice[i][1], WalkUpMatrice[i][2], WalkUpMatrice[i][3], LFR, 0);
    SetLeg(WalkUpMatrice[i][0], WalkUpMatrice[i][1], -WalkUpMatrice[i][2], WalkUpMatrice[i][3], LML, 1);
    SetLeg(WalkUpMatrice[i][0], WalkUpMatrice[i][1], WalkUpMatrice[i][2], WalkUpMatrice[i][3], LBR, 0);
    
    //LEG
    if(i == 2 || i == 3) {

      SetLeg(WalkBottomMatrice[i-1][0], WalkBottomMatrice[i-1][1], -WalkBottomMatrice[i-1][2], WalkBottomMatrice[i-1][3], LFL, 1);
      SetLeg(WalkBottomMatrice[i-1][0], WalkBottomMatrice[i-1][1], WalkBottomMatrice[i-1][2], WalkBottomMatrice[i-1][3], LMR, 0);
      SetLeg(WalkBottomMatrice[i-1][0], WalkBottomMatrice[i-1][1], -WalkBottomMatrice[i-1][2], WalkBottomMatrice[i-1][3], LBL, 1);

    }
    delay(WalkUpMatrice[i][4]);
  }
}
