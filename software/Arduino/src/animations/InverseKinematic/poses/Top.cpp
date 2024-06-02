#include "../../../functions/Algo.h"
#include "../../../config/config.h"
#include "../../../config/Angles.h"
#include "../../../functions/servo.h"

// Fonction principale pour ajuster la hauteur des pattes en fonction des mouvements
void ChangeTop(float speed, float Top) {
    float SPEED = 100 * speed;
    int numLegs = 6;

    // Matrice de positions de base pour chaque patte
    float RouliMatrix[numLegs][3] = {
        {PX, PZ + Top, PY}, // LFL
        {PX, PZ + Top, PY}, // LML
        {PX, PZ + Top, PY}, // LBL
        {PX, PZ + Top, PY}, // RFL
        {PX, PZ + Top, PY}, // RML
        {PX, PZ + Top, PY}  // RBL
    };

    int legIndices[] = {0, 1, 2, 3, 4, 5};

    float targetX[numLegs], targetZ[numLegs], targetY[numLegs];

    for (int leg = 0; leg < numLegs; ++leg) {
        targetX[leg] = RouliMatrix[leg][0];
        targetZ[leg] = RouliMatrix[leg][1];
        targetY[leg] = RouliMatrix[leg][2];
    }

    moveLegsMatrices(legIndices, targetX, targetZ, targetY, numLegs, SPEED);
    PZ = PZ+Top;
}
