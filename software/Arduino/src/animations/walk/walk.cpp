#include "../../config/config.h"
#include "../../config/Angles.h"
#include "../../functions/servo.h"

// R: Rayon cercle virtuel: 5cm (extension de la patte lors de la marche).
// Matrice de positions dans un environnement 3D (x, z, y, ms, attente) => hauteur, extension, rotation coxa en cm, temps du mouvement, temps d'attente une fois le mouvement fait.

float R = 2.5;

void moveLegsSmoothlyWalk(int legIndices[], int legIndices2[], float targetX, float targetZ, float targetY, float targetX2, float targetZ2, float targetY2, int duration) {
    float startX[3], startZ[3], startY[3];
    float startX2[3], startZ2[3], startY2[3];

    for (int i = 0; i < 3; ++i) {
        startX[i] = CurrentPosition[legIndices[i]].x;
        startZ[i] = CurrentPosition[legIndices[i]].z;
        startY[i] = CurrentPosition[legIndices[i]].y;
    }
    for (int i = 0; i < 3; ++i) {
        startX2[i] = CurrentPosition[legIndices2[i]].x;
        startZ2[i] = CurrentPosition[legIndices2[i]].z;
        startY2[i] = CurrentPosition[legIndices2[i]].y;
    }

    unsigned long startTime = millis();
    unsigned long currentTime = startTime;

    while (currentTime - startTime < duration) {
        currentTime = millis();
        float progress = (float)(currentTime - startTime) / duration;

        for (int i = 0; i < 3; ++i) {
            float newZ = startZ[i] + (targetZ - startZ[i]) * progress;
            float newX = startX[i] + (targetX - startX[i]) * progress;
            float newY = startY[i] + (targetY - startY[i]) * progress;

            float newZ2 = startZ2[i] + (targetZ2 - startZ2[i]) * progress;
            float newX2 = startX2[i] + (targetX2 - startX2[i]) * progress;
            float newY2 = startY2[i] + (targetY2 - startY2[i]) * progress;

            int* leg = legs[legIndices[i]];
            int address = (legIndices[i] < 3) ? 1 : 0;
            int* leg2 = legs[legIndices2[i]];
            int address2 = (legIndices2[i] < 3) ? 1 : 0;
          
            setLeg(newX, newZ, newY, 0, leg, address);
            setLeg(newX2, newZ2, newY2, 0, leg2, address2);
        }

        delay(1); // Petite pause pour assurer la fluidité du mouvement
    }

    // Mise à jour de la position finale
    for (int i = 0; i < 3; ++i) {
        setLeg(targetX, targetZ, targetY, 0, legs[legIndices[i]], (legIndices[i] < 3) ? 1 : 0);
        
        // Mise à jour de la position actuelle
        CurrentPosition[legIndices[i]].z = targetZ;
        CurrentPosition[legIndices[i]].x = targetX;
        CurrentPosition[legIndices[i]].y = targetY;
    }
    for (int i = 0; i < 3; ++i) {
        setLeg(targetX2, targetZ2, targetY2, 0, legs[legIndices2[i]], (legIndices2[i] < 3) ? 1 : 0);
        
        // Mise à jour de la position actuelle
        CurrentPosition[legIndices2[i]].z = targetZ2;
        CurrentPosition[legIndices2[i]].x = targetX2;
        CurrentPosition[legIndices2[i]].y = targetY2;
    }
}

void Walk(float speed) {
    float SPEED = 100 * speed;

    float WalkUpMatrice[][5] = {
        {PX, PZ, -2*R, SPEED, 0},
        {PX, PZ, -2*R, SPEED, 0},
        {PX, PZ-3, PY, SPEED, 0},
        {PX, PZ-3, R, SPEED, 0},
        {PX, PZ, 2 * R, SPEED, 0},
    };
    float WalkBottomMatrice[][5] = {
        {PX, PZ-3, -2*R, SPEED, 20},
        {PX, PZ, -2 * R, SPEED, 20},
        {PX, PZ, PY, SPEED, 20},
        {PX, PZ, 2 * R, SPEED, 20},
        {PX, PZ-3, PY, SPEED, 20},
    };

    for (int i = 0; i < sizeof(WalkUpMatrice) / sizeof(WalkUpMatrice[0]); ++i) {
        /*
        {
          0: LFL -> leg front left;
          1: LML -> leg back left;
          2: LBL -> leg middle left;
          3: LFR -> leg front right;
          4: LMR -> leg middle right;
          5: RBL -> leg back right;
        }
        */

        int legIndices[] = {0, 4, 2};
        int legIndicesBottom[] = {3, 1, 5};

        moveLegsSmoothlyWalk(legIndices, legIndicesBottom, WalkUpMatrice[i][0], WalkUpMatrice[i][1], WalkUpMatrice[i][2], WalkBottomMatrice[i][0], WalkBottomMatrice[i][1], WalkBottomMatrice[i][2], WalkUpMatrice[i][3]);

        delay(WalkUpMatrice[i][4]);
    }
}
