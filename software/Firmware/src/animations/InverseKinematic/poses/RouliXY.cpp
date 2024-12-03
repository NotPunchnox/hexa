#include "../../../functions/Algo.h"
#include "../../../config/config.h"
#include "../../../config/Angles.h"
#include "../../../functions/servo.h"
#include <Arduino.h>

// Fonction principale pour ajuster la hauteur des pattes en fonction des mouvements X et Y
void RouliXY(float speed, float X, float Y, float Top, float Bottom, float Left, float Right) {
    float SPEED = 100 * speed;
    int numLegs = 6;

    // Déclaration de la matrice
    float RouliMatrix[numLegs][3];


    for (int leg = 0; leg < numLegs; ++leg) {
        if (leg < 3) {
            RouliMatrix[leg][0] = PX - X;
        } else {
            RouliMatrix[leg][0] = PX + X;
        }
        
        // Attribution de l'axe Y & alterner Y positif et négatif pour chaque paire de pattes
        RouliMatrix[leg][2] = PY + (leg % 2 == 0 ? Y : -Y);

        // Définir la taille de l'axe Z avant de faire les calculs de rouli
        RouliMatrix[leg][1] = TARGET_Z[leg];

        // Calcul pour l'axe Z de chaque patte
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

    // Indices des pattes
    int legIndices[] = {0, 1, 2, 3, 4, 5};

    float targetX[numLegs], targetZ[numLegs], targetY[numLegs];

    for (int leg = 0; leg < numLegs; ++leg) {
        targetX[leg] = RouliMatrix[leg][0];
        targetZ[leg] = RouliMatrix[leg][1];
        targetY[leg] = RouliMatrix[leg][2];

        TARGET_X[leg] = targetX[leg];
        TARGET_Y[leg] = targetY[leg];
        TARGET_Z[leg] = targetZ[leg];
    }

    moveLegsMatrices(legIndices, targetX, targetZ, targetY, numLegs, SPEED);
    Serial.println("Animation:RouliXY:terminé");
}
