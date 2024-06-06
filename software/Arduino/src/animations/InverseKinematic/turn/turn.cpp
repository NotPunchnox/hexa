#include "../../../config/config.h"
#include "../../../config/Angles.h"
#include "../../../functions/servo.h"
#include "../rouli/rouli.h"

// Fonction principale pour marcher vers la gauche
void Turn(String sens, float speed, int cycles){
    float R = 2.5;
    float SPEED = 200 * speed;
    int numLegs = 6;
    int numPoses = 4;

    // Matrices de positions pour chaque patte
    float TurnMatrice[numLegs][numPoses][3] = {
        // {x, z, y} pour chaque patte {LFL, LML, LBL, LFR, LMR, LBR}
        {{PX, PZ, PY}, {PX - (2*R), PZ-3, PY}, {PX - (2*R), PZ, PY}, {PX, PZ, PY}},

        {{PX, PZ, PY}, {PX, PZ, PY}, {PX, PZ, PY}, {PX, PZ, PY}},

        {{PX, PZ, PY}, {PX - (2*R), PZ-3, PY + (2*R)}, {PX - (2*R), PZ, PY}, {PX, PZ, PY}},


        {{PX, PZ, PY}, {PX, PZ, PY}, {PX, PZ, PY}, {PX, PZ, PY}},

        {{PX, PZ, PY}, {PX , PZ-3, PY-(2*R)}, {PX - (2*R), PZ, PY}, {PX, PZ, PY}},

        {{PX, PZ, PY}, {PX, PZ, PY}, {PX, PZ, PY}, {PX, PZ, PY}},
    };

    for (int poseIndex = 0; poseIndex < numPoses; ++poseIndex) {
        int legIndices[] = {0, 1, 2, 3, 4, 5};
        float targetX[numLegs], targetZ[numLegs], targetY[numLegs];

        for (int i = 0; i < numLegs; ++i) {
            targetX[i] = TurnMatrice[i][poseIndex][0];
            targetZ[i] = TurnMatrice[i][poseIndex][1];
            targetY[i] = TurnMatrice[i][poseIndex][2];
        }
        moveLegsMatrices(legIndices, targetX, targetZ, targetY, numLegs, SPEED)
    }
}
