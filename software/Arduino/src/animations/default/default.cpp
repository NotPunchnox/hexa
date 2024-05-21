#include "../../config/config.h"
#include "../../config/Angles.h"
#include "../../functions/Algo.h"
#include <Arduino.h>
#include "../../functions/servo.h"
#include "../walk/walk.h"

// Matrice de positions dans un environnement 3D (x, z, y, ms, attente)
float DefaultMatrice[][5] = {
  {PX, PZ, PY, 0, 3000}, // Patte par défaut
  {PX, PZ-4, PY-5, 400, 40}, // Patte par défaut
  {PX, PZ, PY-5, 200, 30}, // Patte par défaut
};
float MoveRandomLeg[][5] = {
    {PX, PZ, PY, 600, 3000}, // Patte par défaut
};

void moveLegSmoothly(int legIndex, float targetX, float targetZ, float targetY, int duration) {
  float startX = CurrentPosition[legIndex].x;
  float startZ = CurrentPosition[legIndex].z;
  float startY = CurrentPosition[legIndex].y;

  unsigned long startTime = millis();
  unsigned long currentTime = startTime;

  while (currentTime - startTime < duration) {
    currentTime = millis();
    float progress = (float)(currentTime - startTime) / duration;

    float newX = startX + (targetX - startX) * progress;
    float newZ = startZ + (targetZ - startZ) * progress;
    float newY = startY + (targetY - startY) * progress;

    int* leg = legs[legIndex];
    int address = (legIndex < 3) ? 1 : 0;

    setLeg(newX, newZ, newY, 0, leg, address);

    delay(1); // Petite pause pour assurer la fluidité du mouvement
  }

  // Mise à jour de la position finale
  setLeg(targetX, targetZ, targetY, 0, legs[legIndex], (legIndex < 3) ? 1 : 0);

  // Mise à jour de la position actuelle
  CurrentPosition[legIndex].x = targetX;
  CurrentPosition[legIndex].z = targetZ;
  CurrentPosition[legIndex].y = targetY;
}

void Default() {
  setLeg(DefaultMatrice[0][0], DefaultMatrice[0][1], DefaultMatrice[0][2], DefaultMatrice[0][3], LFL, 1);
  setLeg(DefaultMatrice[0][0], DefaultMatrice[0][1], DefaultMatrice[0][2], DefaultMatrice[0][3], LBL, 1);
  setLeg(DefaultMatrice[0][0], DefaultMatrice[0][1], DefaultMatrice[0][2], DefaultMatrice[0][3], LML, 1);

  setLeg(DefaultMatrice[0][0], DefaultMatrice[0][1], DefaultMatrice[0][2], DefaultMatrice[0][3], LFR, 0);
  setLeg(DefaultMatrice[0][0], DefaultMatrice[0][1], DefaultMatrice[0][2], DefaultMatrice[0][3], LMR, 0);
  setLeg(DefaultMatrice[0][0], DefaultMatrice[0][1], DefaultMatrice[0][2], DefaultMatrice[0][3], LBR, 0);

  delay(DefaultMatrice[0][4]);

    for (int s = 0; s < 1; ++s) {
        int randomLegIndex = random(0, 6);  // Sélectionner une patte aléatoirement
        int randomWaitDelay = random(0, 9) * 4000;  // Sélectionner une durée aléatoirement
        int randomLegMove = random(0, 1);  // Sélectionner une patte aléatoirement

        for (int i = 1; i < sizeof(DefaultMatrice) / sizeof(DefaultMatrice[0]); ++i) {
            moveLegSmoothly(
                randomLegIndex,
                randomLegMove == 0 ? DefaultMatrice[i][0] : MoveRandomLeg[0][0],
                randomLegMove == 0 ? DefaultMatrice[i][1] : MoveRandomLeg[0][1],
                randomLegMove == 0 ? DefaultMatrice[i][2] : MoveRandomLeg[0][2],
                randomLegMove == 0 ? DefaultMatrice[i][3] : MoveRandomLeg[0][3]
            );
            delay(DefaultMatrice[i][4]);
        }
        delay(randomWaitDelay);
    }
}