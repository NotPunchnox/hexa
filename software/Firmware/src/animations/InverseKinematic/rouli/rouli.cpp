#include "../../../functions/Algo.h"
#include "../../../config/config.h"
#include "../../../config/Angles.h"
#include "../../../functions/servo.h"

// Fonction principale pour ajuster la hauteur des pattes en fonction des mouvements
void Rouli(float speed, float Top, float Bottom, float Left, float Right) {
    float SPEED = 200 * speed;
    int numLegs = 6;

    // Matrice de positions de base pour chaque patte
    float RouliMatrix[numLegs][3] = {
        {PX, PZ, PY}, // LFL
        {PX, PZ, PY}, // LML
        {PX, PZ, PY}, // LBL
        {PX, PZ, PY}, // RFL
        {PX, PZ, PY}, // RML
        {PX, PZ, PY}  // RBL
    };

    // Appliquer les transformations uniquement sur l'axe Z
    for (int leg = 0; leg < numLegs; ++leg) {
        /*
        LOGIQUE:
        Patte 0 % 3 = 0
        Patte 1 % 3 = 1
        Patte 2 % 3 = 2
        Patte 3 % 3 = 0
        Patte 4 % 3 = 1
        Patte 5 % 3 = 2
        */
        if (Top > 0) {
            RouliMatrix[leg][1] += (leg % 3 == 0) ? Top : ((leg % 3 == 2) ? -Top : 0);
        }
        if (Bottom > 0) {
            RouliMatrix[leg][1] += (leg % 3 == 0) ? -Bottom : ((leg % 3 == 2) ? Bottom : 0);
        }
        if (Left > 0) {
            RouliMatrix[leg][1] += (leg < 3) ? Left : -Left;
        }
        if (Right > 0) {
            RouliMatrix[leg][1] += (leg < 3) ? -Right : Right;
        }
    }

    int legIndices[] = {0, 1, 2, 3, 4, 5};

    float targetX[numLegs], targetZ[numLegs], targetY[numLegs];

    for (int leg = 0; leg < numLegs; ++leg) {
        targetX[leg] = RouliMatrix[leg][0];
        targetZ[leg] = RouliMatrix[leg][1];
        targetY[leg] = RouliMatrix[leg][2];
    }

    moveLegsMatrices(legIndices, targetX, targetZ, targetY, numLegs, SPEED);
}