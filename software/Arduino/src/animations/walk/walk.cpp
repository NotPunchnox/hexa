#include "../../config/config.h"
#include "../../functions/Algo.h"
#include <Arduino.h>
#include "../../functions/servo.h"
#include "../sleep/sleep.h"

struct LegPosition {
  int servoChannels[3];
  float x;
  float y;
  float z;
  int address;
  int duree;
};

void Move(LegPosition legs[]) {
  LegConfig LEGS[sizeof(legs)];

  for(int i = 0; i < sizeof(legs); ++i) {
    LegPosition leg = legs[i];
    int* LEG = leg.servoChannels;

    if(LEG == LFL || LEG == LBR) {
      leg.y += 11;
    }
    if(LEG == LFR || LEG == LBL) {
      leg.y -= 11;
    }

    LegAngles res = Algo(leg.x, leg.z, leg.y, leg.duree);

    float angles[3];

    if (LEG == LML || LEG == LFL || LEG == LBL) { // Si c'est une patte de gauche, il faut inverser les angles tibia et fémur
      angles[0] = res.AngleTibia;
      angles[1] = 180 - res.AngleFemur;
      angles[2] = res.AngleCoxa;
    } else {
      angles[0] = 180 - res.AngleTibia;
      angles[1] = res.AngleFemur;
      angles[2] = res.AngleCoxa;
    }

    LEGS[i].address = leg.address;
    LEGS[i].coxa = angles[2];
    LEGS[i].femur = angles[1];
    LEGS[i].tibia = angles[0];

    for (int j = 0; j < 3; ++j) {
      LEGS[i].servoChannels[j] = leg.servoChannels[j];
    }

    LEGS[i].duree = leg.duree;
  }

  setLegs(LEGS);
}

// R: Rayon cercle virtuel: 5cm (extension de la patte lors de la marche).
// Matrice de positions dans un environnement 3D (x, z, y, ms, attente) => hauteur, extension, rotation coxa en cm, temps du mouvement, temps d'attente une fois le mouvement fait.

float R = 2.5;

float WalkUpMatrice[][5] = {
  {PX, PZ, PY, 20, 0},      // Patte par défaut
  {PX, 1, PY, 600, 0},      // Patte levée sur l'axe Z
  {PX, 1, 2*R, 200, 0},     // Avancer la patte sur l'axe Y
  {PX, PZ, 2*R, 600, 0},    // Reposer la patte sur l'axe Z
  {PX, PZ, R, 500, 0},
  {PX, PZ, -R, 500, 0},
  {PX, PZ-5, -R, 500, 0},
  {PX, PZ, PY, 500, 0},
};

float WalkBottomMatrice[][5] = {
  {PX, PZ, PY, 500, 1000},  // Patte par défaut
  {PX, PZ, 2*R, 700, 200},  // Patte recule sur l'axe Y
};

void Walk() {
  for (int i = 0; i < sizeof(WalkUpMatrice) / sizeof(WalkUpMatrice[0]); ++i) {
    int ReverseMatrice = sizeof(WalkUpMatrice) / sizeof(WalkUpMatrice[0]) - i;

    LegPosition legs[2];
    legs[0].address = 1;
    legs[0].duree = WalkUpMatrice[i][3];
    memcpy(legs[0].servoChannels, LFL, sizeof(LFL));
    legs[0].x = WalkUpMatrice[i][0];
    legs[0].z = WalkUpMatrice[i][1];
    legs[0].y = WalkUpMatrice[i][2];

    legs[1].address = 0;
    legs[1].duree = WalkUpMatrice[i][3];
    memcpy(legs[1].servoChannels, LBR, sizeof(LBR));
    legs[1].x = WalkUpMatrice[i][0];
    legs[1].z = WalkUpMatrice[i][1];
    legs[1].y = WalkUpMatrice[i][2];

    Move(legs);
    
    delay(WalkUpMatrice[i][4]);
  }
}
