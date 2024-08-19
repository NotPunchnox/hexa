#include "../../config/config.h"
#include "../../config/Angles.h"
#include "../../functions/servo.h"

// Matrice de positions dans un environnement 3D (x, z, y, ms, attente) => hauteur, extension, rotation coxa en cm, temps du mouvement, temps d'attente une fois le mouvement fait.
void Walk(float speed, float X, float Y) {
    float SPEED = 50 * speed;
    int numLegs = 6;
    int numSteps = 4; // Nombre de pas par séquence dans l'animation.
    int Z = 3;

    //X < 0 = mouve to left
    //X > 0 = move to right
    //Y < 0 = move to bottom
    //Y > 0 = move to up

    float WalkMatrice[numLegs][numSteps][3] = {
        {{PX + X, PZ, Y*-1}, {PX + X, PZ, Y*-1}, {PX + (X*-1), PZ-Z, Y}, {PX + (X*-1), PZ, Y}},  // LFL

        {{PX + (X*-1), PZ-Z, Y*-1}, {PX + (X*-1), PZ, Y*-1}, {PX + X, PZ, Y}, {PX + X, PZ, Y}}, // LML a

        {{PX + X, PZ, Y*-1}, {PX + X, PZ, Y*-1}, {PX + (X*-1), PZ-Z, Y}, {PX + (X*-1), PZ, Y}}, // LBL

        {{PX + X, PZ-Z, Y*-1}, {PX + X, PZ, Y*-1}, {PX + (X*-1), PZ, Y}, {PX + (X*-1), PZ, Y}}, // LFR a

        {{PX + (X*-1), PZ, Y*-1}, {PX + (X*-1), PZ, Y*-1}, {PX + X, PZ-Z, Y}, {PX + X, PZ, Y}}, // LMR

        {{PX + X, PZ-Z, Y*-1}, {PX + X, PZ, Y*-1}, {PX + (X*-1), PZ, Y}, {PX + (X*-1), PZ, Y}} // LBR a
    };

    for (int stepIndex = 0; stepIndex < numSteps; ++stepIndex) {
        int legIndices[] = {0, 1, 2, 3, 4, 5};
        float targetX[numLegs], targetZ[numLegs], targetY[numLegs];

        for (int i = 0; i < numLegs; ++i) {
            targetX[i] = WalkMatrice[i][stepIndex][0];
            targetZ[i] = WalkMatrice[i][stepIndex][1];
            targetY[i] = WalkMatrice[i][stepIndex][2];
        }

        moveLegsMatrices(legIndices, targetX, targetZ, targetY, numLegs, SPEED);
        delay(10);
    }
}
