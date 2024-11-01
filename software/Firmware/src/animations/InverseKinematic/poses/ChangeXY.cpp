#include "../../../functions/Algo.h"
#include "../../../config/config.h"
#include "../../../config/Angles.h"
#include "../../../functions/servo.h"
#include <Arduino.h>

// Fonction principale pour ajuster la hauteur des pattes en fonction des mouvements X et Y
void ChangeXY(float speed, float X, float Y) {
    float SPEED = 100 * speed;
    int numLegs = 6;

    float RouliMatrix[numLegs][3];


    float PX_L = PX - X;
    float PX_R = PX + X;

    for (int leg = 0; leg < numLegs; ++leg) {
        if (leg < 3) {
            RouliMatrix[leg][0] = PX_L;
        } else {
            RouliMatrix[leg][0] = PX_R;
        }
        RouliMatrix[leg][1] = PZ;
        RouliMatrix[leg][2] = PY + (leg % 2 == 0 ? Y : -Y); // Alterner Y positif et négatif pour chaque paire de pattes
    }

    // Indices des pattes
    int legIndices[] = {0, 1, 2, 3, 4, 5};

    float targetX[numLegs], targetZ[numLegs], targetY[numLegs];

    for (int leg = 0; leg < numLegs; ++leg) {
        targetX[leg] = RouliMatrix[leg][0];
        targetZ[leg] = RouliMatrix[leg][1];
        targetY[leg] = RouliMatrix[leg][2];
    }

    moveLegsMatrices(legIndices, targetX, targetZ, targetY, numLegs, SPEED);
    Serial.println("Animation:ChangeXY:terminé");
}
