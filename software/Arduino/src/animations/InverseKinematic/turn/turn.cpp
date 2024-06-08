#include "../../../config/config.h"
#include "../../../config/Angles.h"
#include "../../../functions/servo.h"
#include "../rouli/rouli.h"

// Fonction principale pour tourner
void Turn(String sens, float speed, int cycles) {
    float R = 2.5; // Rayon de la marche
    float SPEED = 200 * speed;
    int numLegs = 6;
    int numSteps = 5; // Nombre de pas par séquence

    // Déterminer le sens de la rotation
    int direction = (sens == "left") ? 1 : -1;

    // Matrices de positions pour chaque patte
    float TurnMatrice[numLegs][numSteps][3] = {
        // {x, z, y} pour chaque patte {LFL, LML, LBL, LFR, LMR, LBR}
        {{PX, PZ, PY}, {PX, PZ, PY}, {PX, PZ-5, PY}, {PX-direction * R, PZ-5, PY}, {PX-direction * R, PZ, PY}}, // LFL
        {{PX, PZ -5, PY + direction * R}, {PX, PZ, PY+direction * R}, {PX, PZ, PY+direction * R}, {PX, PZ, PY-direction * R}, {PX, PZ, PY-direction * R}}, // LML
        {{PX, PZ, PY}, {PX, PZ, PY}, {PX, PZ-5, PY}, {PX, PZ-5, PY-direction * R}, {PX, PZ, PY-direction * R}}, // LBL
        
        {{PX - direction * R, PZ-5, PY}, {PX-direction * R, PZ, PY}, {PX-direction * R, PZ, PY}, {PX+direction * R, PZ, PY}, {PX+direction * R, PZ, PY}}, // LFR
        {{PX, PZ, PY}, {PX, PZ, PY}, {PX, PZ-5, PY}, {PX, PZ-5, PY+direction * R}, {PX, PZ, PY+direction * R}}, // LMR
        {{PX, PZ -5, PY - direction * R}, {PX, PZ, PY-direction * R}, {PX, PZ, PY-direction * R}, {PX, PZ, PY+direction * R}, {PX, PZ, PY+direction * R}} // LBR
    };

    for (int cycle = 0; cycle < cycles; ++cycle) {
        for (int stepIndex = 0; stepIndex < numSteps; ++stepIndex) {
            int legIndices[] = {0, 1, 2, 3, 4, 5};
            float targetX[numLegs], targetZ[numLegs], targetY[numLegs];

            for (int i = 0; i < numLegs; ++i) {
                targetX[i] = TurnMatrice[i][stepIndex][0];
                targetZ[i] = TurnMatrice[i][stepIndex][1];
                targetY[i] = TurnMatrice[i][stepIndex][2];
            }

            moveLegsMatrices(legIndices, targetX, targetZ, targetY, numLegs, SPEED);
            //delay(100); // Petite pause pour assurer la fluidité du mouvement
        }
    }
}