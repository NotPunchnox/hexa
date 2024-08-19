#include "../../../functions/Algo.h"
#include "../../../config/config.h"
#include "../../../config/Angles.h"
#include "../../../functions/servo.h"

// Fonction principale pour ajuster la hauteur des pattes en fonction des mouvements
void ChangeY(float speed, float Y) {
    float SPEED = 100 * speed;
    int numLegs = 6;

    // Matrice de positions de base pour chaque patte
    float RouliMatrix[numLegs][3] = {
        {PX, PZ, PY+Y}, // LFL
        {PX, PZ, PY+Y * -1}, // LML
        {PX, PZ, PY+Y}, // LBL
        {PX, PZ, PY+Y*-1}, // RFL
        {PX, PZ, PY+Y}, // RML
        {PX, PZ, PY+Y*-1}  // RBL
    };

    int legIndices[] = {0, 1, 2, 3, 4, 5};

    float targetX[numLegs], targetZ[numLegs], targetY[numLegs];

    for (int leg = 0; leg < numLegs; ++leg) {
        targetX[leg] = RouliMatrix[leg][0];
        targetZ[leg] = RouliMatrix[leg][1];
        targetY[leg] = RouliMatrix[leg][2];
    }

    moveLegsMatrices(legIndices, targetX, targetZ, targetY, numLegs, SPEED);
}
