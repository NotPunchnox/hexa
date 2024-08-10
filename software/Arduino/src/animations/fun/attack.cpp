#include "../../../config/config.h"
#include "../../../config/Angles.h"
#include "../../../functions/servo.h"
#include "../rouli/rouli.h"

// Fonction principale pour l'attaque
void AttackMove(float speed) {
    float SPEED = 200 * speed;
    int numLegs = 6;
    int numSteps = 3; // Nombre de pas par séquence dans la matrice Attack

    float AttackMatrice[numLegs][numSteps][3] = {
        {{10, 9, 0}, {10, 9, 0}, {10, 9, 0}},  // Step 1
        {{10, 6, -10}, {10, 10, -10}, {10, 6, 10}}, // Step 2
        {{10, 9, 0}, {10, 7.5, 0}, {10, 7.6, 0}}, // Step 3
        {{10, 9, 0}, {10, 9, 0}, {10, 7.5, 0}}, // LFR
        {{10, 6, 10}, {10, 10, 10}, {10, 10, 10}}, // LMR
        {{10, 9, 0}, {10, 7.6, 0}, {10, 7.6, 0}} // LBR
    };

    for (int stepIndex = 0; stepIndex < numSteps; ++stepIndex) {
        int legIndices[] = {0, 1, 2, 3, 4, 5};
        float targetX[numLegs], targetZ[numLegs], targetY[numLegs];

        for (int i = 0; i < numLegs; ++i) {
            targetX[i] = AttackMatrice[i][stepIndex][0];
            targetZ[i] = AttackMatrice[i][stepIndex][1];
            targetY[i] = AttackMatrice[i][stepIndex][2];
        }

        moveLegsMatrices(legIndices, targetX, targetZ, targetY, numLegs, SPEED);
    }
}
