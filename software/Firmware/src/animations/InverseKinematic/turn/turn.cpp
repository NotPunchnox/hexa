#include "../../../config/config.h"
#include "../../../config/Angles.h"
#include "../../../functions/servo.h"

// Fonction principale pour tourner
void Turn(String sens, float speed, int cycles, float r) {
    float R = r; // Rayon de la marche
    float SPEED = 200 * speed;
    int numLegs = 6;
    int numSteps = 4; // Nombre de pas par séquence
    int Z = 3;

    // Déterminer le sens de la rotation
    int direction = (sens == "left") ? 1 : -1;

    // Matrices de positions pour chaque patte
    float TurnMatrice[numLegs][numSteps][3] = {
        // {x, z, y} pour chaque patte {LFL, LML, LBL, LFR, LMR, LBR}
        //Colonne = Une séquence
        //Ligne = patte

        {{PX + R * direction, PZ,   PY - R * direction},  {PX + R * direction, PZ, PY - R * direction}, {PX - R * direction, PZ - Z, PY + R * direction}, {PX - R * direction, PZ, PY + R * direction} }, // LFL
        {{PX + R * direction, PZ - Z, PY - R * direction},  {PX + R * direction, PZ, PY - R * direction}, {PX - R * direction, PZ,   PY + R * direction}, {PX - R * direction, PZ, PY + R * direction} }, // LML
        {{PX - R * direction, PZ,   PY - R * direction},  {PX - R * direction, PZ, PY - R * direction}, {PX + R * direction, PZ - Z, PY + R * direction}, {PX + R * direction, PZ, PY + R * direction} }, // LBL

        {{PX + R * direction, PZ - Z, PY - R * direction},  {PX + R * direction, PZ, PY - R * direction}, {PX - R * direction, PZ,   PY + R * direction}, {PX - R * direction, PZ, PY + R * direction} }, // LFR
        {{PX - R * direction, PZ,   PY + R * direction},  {PX - R * direction, PZ, PY + R * direction}, {PX + R * direction, PZ - Z, PY - R * direction}, {PX + R * direction, PZ, PY - R * direction} }, // LMR
        {{PX - R * direction, PZ - Z, PY - R * direction},  {PX - R * direction, PZ, PY - R * direction}, {PX + R * direction, PZ,   PY + R * direction}, {PX + R * direction, PZ, PY + R * direction} }  // LBR
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
            delay(10);
        }
        Serial.println("Turn:" + String(cycle) + ":" + String(cycles));
    }
    Serial.println("Animation:Turn:terminé");
}
