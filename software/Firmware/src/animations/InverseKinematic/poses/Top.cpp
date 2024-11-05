#include "../../../functions/Algo.h"
#include "../../../config/config.h"
#include "../../../config/Angles.h"
#include "../../../functions/servo.h"

// Fonction principale pour ajuster la hauteur des pattes en fonction des mouvements
void ChangeTop(float speed, float Top) {
    float SPEED = 100 * speed;
    int numLegs = 6;
    PZ = PZ+Top;

    // Matrice de positions de base pour chaque patte
    float RouliMatrix[numLegs][3] = {
        {PX, TARGET_Z[0]+Top, PY}, // LFL
        {PX, TARGET_Z[1]+Top, PY}, // LML
        {PX, TARGET_Z[2]+Top, PY}, // LBL
        {PX, TARGET_Z[3]+Top, PY}, // RFL
        {PX, TARGET_Z[4]+Top, PY}, // RML
        {PX, TARGET_Z[5]+Top, PY}  // RBL
    };

    int legIndices[] = {0, 1, 2, 3, 4, 5};

    float targetX[numLegs], targetZ[numLegs], targetY[numLegs];

    for (int leg = 0; leg < numLegs; ++leg) {
        targetX[leg] = RouliMatrix[leg][0];
        targetZ[leg] = RouliMatrix[leg][1];
        targetY[leg] = RouliMatrix[leg][2];
    }

    moveLegsMatrices(legIndices, targetX, targetZ, targetY, numLegs, SPEED);
    Serial.println("Animation:ChangeTop:terminé");
}
