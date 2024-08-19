#include "../../../config/config.h"
#include "../../../config/Angles.h"
#include "../../../functions/servo.h"

// Fonction principale pour tourner
void Turn(String sens, float speed, int cycles) {
    float R = 3; // Rayon de la marche
    float SPEED = 200 * speed;
    int numLegs = 6;
    int numSteps = 4; // Nombre de pas par séquence

    // Déterminer le sens de la rotation
    int direction = (sens == "left") ? 1 : -1;

    // Matrices de positions pour chaque patte
    float TurnMatrice[numLegs][numSteps][3] = {
        // {x, z, y} pour chaque patte {LFL, LML, LBL, LFR, LMR, LBR}
        //Colonne = Une séquence
        //Ligne = patte
        {{PX, PZ, PY}, {PX, PZ-3, PY-direction * R}, {PX, PZ, PY-direction * R*2}, {PX, PZ, PY}}, // LFL
        {{PX, PZ, PY}, {PX, PZ, PY - direction *R*2}, {PX, PZ-3, PY}, {PX, PZ-3, PY}},
        {{PX, PZ, PY}, {PX-direction * R, PZ-3, PY}, {PX-direction * R*2, PZ, PY}, {PX, PZ, PY}},

        {{PX, PZ, PY}, {PX, PZ, PY+direction*R*2}, {PX, PZ-3, PY}, {PX, PZ-3, PY}},
        {{PX, PZ, PY}, {PX, PZ-3, PY+direction*R}, {PX, PZ, PY+direction*R*2}, {PX, PZ, PY}},
        {{PX, PZ, PY}, {PX-direction*R*2, PZ, PY}, {PX, PZ-3, PY}, {PX, PZ-3, PY}},
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