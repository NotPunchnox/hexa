#include "../../config/config.h"
#include "../../config/Angles.h"
#include "../../functions/servo.h"

// Fonction pour déplacer les pattes en douceur
void moveLegsSmoothly(int legIndices[], float targetX[], float targetZ[], float targetY[], int numLegs, int duration) {
    float startX[numLegs], startZ[numLegs], startY[numLegs];

    for (int i = 0; i < numLegs; ++i) {
        startX[i] = CurrentPosition[legIndices[i]].x;
        startZ[i] = CurrentPosition[legIndices[i]].z;
        startY[i] = CurrentPosition[legIndices[i]].y;
    }

    unsigned long startTime = millis();
    unsigned long currentTime = startTime;

    while (currentTime - startTime < duration) {
        currentTime = millis();
        float progress = (float)(currentTime - startTime) / duration;

        for (int i = 0; i < numLegs; ++i) {
            float newX = startX[i] + (targetX[i] - startX[i]) * progress;
            float newZ = startZ[i] + (targetZ[i] - startZ[i]) * progress;
            float newY = startY[i] + (targetY[i] - startY[i]) * progress;

            int* leg = legs[legIndices[i]];
            int address = (legIndices[i] < 3) ? 1 : 0;

            setLeg(newX, newZ, newY, 0, leg, address);
        }

        delay(1); // Petite pause pour assurer la fluidité du mouvement
    }

    // Mise à jour de la position finale
    for (int i = 0; i < numLegs; ++i) {
        setLeg(targetX[i], targetZ[i], targetY[i], 0, legs[legIndices[i]], (legIndices[i] < 3) ? 1 : 0);
        CurrentPosition[legIndices[i]].x = targetX[i];
        CurrentPosition[legIndices[i]].z = targetZ[i];
        CurrentPosition[legIndices[i]].y = targetY[i];
    }
}

// Fonction principale pour marcher vers la gauche
void WalkLeft(float speed) {
    float R = 2.5;
    float SPEED = 200 * speed;
    int numLegs = 6;

    // Matrices de positions pour chaque patte
    float WalkMatrice[numLegs][3][3] = {
        // {x, z, y} pour chaque patte {LFL, LBL, LML, LFR, LMR, LBR}
        {{PX, PZ, PY}, {PX + R, PZ - 3, PY}, {PX - 2 * R, PZ, PY}},
        {{PX, PZ, PY}, {PX + R, PZ - 3, PY}, {PX - 2 * R, PZ, PY}},
        {{PX, PZ, PY}, {PX + R, PZ - 3, PY}, {PX - 2 * R, PZ, PY}},

        {{PX, PZ, PY}, {PX - R, PZ, PY}, {PX + 2 * R, PZ, PY}},
        {{PX, PZ, PY}, {PX - R, PZ, PY}, {PX + 2 * R, PZ, PY}},
        {{PX, PZ, PY}, {PX - R, PZ, PY}, {PX + 2 * R, PZ, PY}},
    };

    for (int i = 0; i < numLegs; ++i) {
        // {LFL, LBL, LML}
        int legIndices[] = {1, 0, 4, 2, 3, 5};

        float targetX[numLegs], targetZ[numLegs], targetY[numLegs];

        for (int j = 0; j < 3; ++j) {
            targetX[j] = WalkMatrice[i][j][0];
            targetZ[j] = WalkMatrice[i][j][1];
            targetY[j] = WalkMatrice[i][j][2];
        }

        moveLegsSmoothly(legIndices, targetX, targetZ, targetY, numLegs, SPEED);
    }
}
