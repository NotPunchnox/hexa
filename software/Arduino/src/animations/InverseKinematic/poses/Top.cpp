#include "../../../functions/Algo.h"
#include "../../../config/config.h"
#include "../../../config/Angles.h"
#include "../../../functions/servo.h"
#include "../rouli/rouli.h"

// Fonction principale pour ajuster la hauteur des pattes en fonction des mouvements
void ChangeTop(float speed, float Top) {
    float SPEED = 100 * speed;
    int numLegs = 6;
    PZ = PZ+Top;

    // Matrice de positions de base pour chaque patte
    float RouliMatrix[numLegs][3] = {
        {PX, PZ, PY}, // LFL
        {PX, PZ, PY}, // LML
        {PX, PZ, PY}, // LBL
        {PX, PZ, PY}, // RFL
        {PX, PZ, PY}, // RML
        {PX, PZ, PY}  // RBL
    };

    int legIndices[] = {0, 1, 2, 3, 4, 5};

    float targetX[numLegs], targetZ[numLegs], targetY[numLegs];

    for (int leg = 0; leg < numLegs; ++leg) {
        targetX[leg] = RouliMatrix[leg][0];
        targetZ[leg] = RouliMatrix[leg][1];
        targetY[leg] = RouliMatrix[leg][2];
    }

    moveUniquePoseLegsSmoothly(legIndices, targetX, targetZ, targetY, numLegs, SPEED);
}
