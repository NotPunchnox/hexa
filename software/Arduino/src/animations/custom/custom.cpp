#include "../../config/config.h"
#include "../../config/Angles.h"
#include "../../functions/servo.h"
#include "../InverseKinematic/rouli/rouli.h"

// Fonction principale pour tourner ou effectuer des mouvements personnalisés
void Custom(float animationMatrix[][4][3], int numSteps, float speed, int cycles) {
    float SPEED = 200 * speed;
    int numLegs = 6;

    for (int cycle = 0; cycle < cycles; ++cycle) {
        for (int stepIndex = 0; stepIndex < numSteps; ++stepIndex) {
            int legIndices[] = {0, 1, 2, 3, 4, 5};
            float targetX[numLegs], targetZ[numLegs], targetY[numLegs];

            for (int i = 0; i < numLegs; ++i) {
                targetX[i] = animationMatrix[i][stepIndex][0];
                targetZ[i] = animationMatrix[i][stepIndex][1];
                targetY[i] = animationMatrix[i][stepIndex][2];
            }

            moveLegsMatrices(legIndices, targetX, targetZ, targetY, numLegs, SPEED);
            delay(100); // Petite pause pour assurer la fluidité du mouvement
        }
    }
}
