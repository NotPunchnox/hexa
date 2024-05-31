#include "../../../functions/Algo.h"
#include "../../../config/config.h"
#include "../../../config/Angles.h"
#include "../../../functions/servo.h"

// Fonction pour déplacer les pattes progressivement avec des coordonnées différentes par pattes
void moveUniquePoseLegsSmoothly(int legIndices[], float targetX[], float targetZ[], float targetY[], int numLegs, int duration) {
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

        delay(1);
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
void Rouli(float speed, float Top, float Bottom, float Left, float Right) {
    float SPEED = 200 * speed;
    int numLegs = 6;
    int numPoses = 2;

    // Matrices de positions pour chaque patte
    float RouliTop[numLegs][numPoses][3] = {
        // {x, z, y} pour chaque patte {LFL, LML, LBL, RFL, RML, RBL}
        {{PX, PZ, PY}, {PX, PZ + Top, PY}},
        {{PX, PZ, PY}, {PX, PZ, PY}},
        {{PX, PZ, PY}, {PX, PZ - Top, PY}},
        {{PX, PZ, PY}, {PX, PZ + Top, PY}},
        {{PX, PZ, PY}, {PX, PZ, PY}},
        {{PX, PZ, PY}, {PX, PZ -Top, PY}},
    };
    float RouliBottom[numLegs][numPoses][3] = {
        // {x, z, y} pour chaque patte {LFL, LML, LBL, RFL, RML, RBL}
        {{PX, PZ, PY}, {PX, PZ-Bottom , PY}},        // LFL
        {{PX, PZ, PY}, {PX, PZ, PY}}, // LML
        {{PX, PZ, PY}, {PX, PZ + Bottom, PY}},             // LBL
        {{PX, PZ, PY}, {PX, PZ-Bottom, PY}},       // RFL
        {{PX, PZ, PY}, {PX, PZ, PY}}, // RML
        {{PX, PZ, PY}, {PX, PZ + Bottom, PY}},             // RBL
    };
    float RouliLeft[numLegs][numPoses][3] = {
        {{PX, PZ, PY}, {PX, PZ+Left, PY}},
        {{PX, PZ, PY}, {PX, PZ+Left, PY}},
        {{PX, PZ, PY}, {PX, PZ+Left, PY}},
        {{PX, PZ, PY}, {PX, PZ-Left, PY}},
        {{PX, PZ, PY}, {PX, PZ-Left, PY}},
        {{PX, PZ, PY}, {PX, PZ-Left, PY}},
    };
    float RouliRight[numLegs][numPoses][3] = {
        {{PX, PZ, PY}, {PX, PZ-Right , PY}},
        {{PX, PZ, PY}, {PX, PZ-Right, PY}},
        {{PX, PZ, PY}, {PX, PZ-Right, PY}},
        {{PX, PZ, PY}, {PX, PZ+Right, PY}},
        {{PX, PZ, PY}, {PX, PZ+Right, PY}},
        {{PX, PZ, PY}, {PX, PZ+Right, PY}},
    };

    for (int pose = 0; pose < numPoses; ++pose) {
        int legIndices[] = {0, 1, 2, 3, 4, 5};

        float targetX[numLegs], targetZ[numLegs], targetY[numLegs];

        for (int leg = 0; leg < numLegs; ++leg) {
            if(Top>0) {
                targetX[leg] = RouliTop[leg][pose][0];
                targetZ[leg] = RouliTop[leg][pose][1];
                targetY[leg] = RouliTop[leg][pose][2];
            } else if(Bottom>0) {
                targetX[leg] = RouliBottom[leg][pose][0];
                targetZ[leg] = RouliBottom[leg][pose][1];
                targetY[leg] = RouliBottom[leg][pose][2];
            } else if (Left>0) {
                targetX[leg] = RouliLeft[leg][pose][0];
                targetZ[leg] = RouliLeft[leg][pose][1];
                targetY[leg] = RouliLeft[leg][pose][2];
            } else if (Right>0) {
                targetX[leg] = RouliRight[leg][pose][0];
                targetZ[leg] = RouliRight[leg][pose][1];
                targetY[leg] = RouliRight[leg][pose][2];
            };
            
        }

        moveUniquePoseLegsSmoothly(legIndices, targetX, targetZ, targetY, numLegs, SPEED);
    }
}
