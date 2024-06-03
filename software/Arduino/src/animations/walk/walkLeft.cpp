#include "../../config/config.h"
#include "../../config/Angles.h"
#include "../../functions/servo.h"
#include "../InverseKinematic/rouli/rouli.h"

// Fonction principale pour marcher vers la gauche
void WalkLeft(float speed) {
    float R = 2.5;
    float SPEED = 200 * speed;
    int numLegs = 6;
    int numPoses = 4;

    // Matrices de positions pour chaque patte
    float WalkMatrice[numLegs][numPoses][3] = {
        // {x, z, y} pour chaque patte {LFL, LML, LBL, LFR, LMR, LBR}
        {{PX, PZ, PY}, {PX + R, PZ-3, PY}, {PX + (2*R), PZ, PY}, {PX - R, PZ, PY}},

        {{PX, PZ, PY}, {PX - R, PZ-3, PY}, {PX - (2*R), PZ, PY}, {PX + R, PZ, PY}},

        {{PX, PZ, PY}, {PX + R, PZ-3, PY}, {PX + (2*R), PZ, PY}, {PX - R, PZ, PY}},


        {{PX, PZ, PY}, {PX - R, PZ, PY}, {PX - (2*R), PZ, PY}, {PX + R, PZ, PY}},

        {{PX, PZ, PY}, {PX + R, PZ, PY}, {PX + (2*R), PZ, PY}, {PX - R, PZ, PY}},

        {{PX, PZ, PY}, {PX - R, PZ, PY}, {PX - (2*R), PZ, PY}, {PX + R, PZ, PY}},
    };

    for (int poseIndex = 0; poseIndex < numPoses; ++poseIndex) {
        int legIndices[] = {0, 4, 2, 3, 1, 5};
        float targetX[numLegs], targetZ[numLegs], targetY[numLegs];

        for (int i = 0; i < numLegs; ++i) {
            targetX[i] = WalkMatrice[i][poseIndex][0];
            targetZ[i] = WalkMatrice[i][poseIndex][1];
            targetY[i] = WalkMatrice[i][poseIndex][2];
        }

        moveUniquePoseLegsSmoothly(legIndices, targetX, targetZ, targetY, numLegs, SPEED);
    }
}
