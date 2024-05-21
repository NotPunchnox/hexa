#include "../../config/config.h"
#include "../../config/Angles.h"
#include "../../functions/Algo.h"
#include <Arduino.h>
#include "../../functions/servo.h"
#include "../sleep/sleep.h"

// Fonction pour déplacer une patte du robot aux coordonnées désirées
void setLeg(float x, float z, float y, int duree, int LEG[3], int address) {
    if (LEG == LFL || LEG == LBR) {
        y += 9;
        x -= 1;
    } else if (LEG == LFR || LEG == LBL) {
        y -= 9;
        x -= 1;
    }

    if (LEG == LMR) {
      y = -y;
    }
    Serial.print("\nx: ");
    Serial.print(x);
    Serial.print(" | y: ");
    Serial.print(y);
    Serial.print(" | z: ");
    Serial.print(z);

    LegAngles res = Algo(x, z, y, duree);

    float angles[3];

    if (LEG == LML || LEG == LFL || LEG == LBL) { // Si c'est une patte gauche, il faut inverser les angles tibia et fémur
        angles[0] = res.AngleTibia;
        angles[1] = 180 - res.AngleFemur;
        angles[2] = res.AngleCoxa;
    } else {
        angles[0] = 180 - res.AngleTibia;
        angles[1] = res.AngleFemur;
        angles[2] = res.AngleCoxa;
    }
    Serial.println(" ");
    Serial.println("\nAngle coxa: ");
    Serial.print(angles[2]);
    Serial.println("\nAngle femur: ");
    Serial.print(angles[1]);
    Serial.println("\nAngle tibia: ");
    Serial.print(angles[0]);

    setServo(LEG, 3, angles, address, duree);
}

// R: Rayon cercle virtuel: 5cm (extension de la patte lors de la marche).
// Matrice de positions dans un environnement 3D (x, z, y, ms, attente) => hauteur, extension, rotation coxa en cm, temps du mouvement, temps d'attente une fois le mouvement fait.

float R = 2.5;

float WalkUpMatrice[][5] = {
    {PX, PZ, PY, 200, 100},     // Patte par défaut
    {PX, 1, PY, 300, 20},       // Patte levée sur l'axe Z
    {PX, 1, R, 300,20},        // Patte levée sur l'axe Z
    {PX, 1, 2 * R, 200, 20}, // Avancer la patte sur l'axe Y
    {PX, PZ, 2 * R, 600, 20}, // Reposer la patte sur l'axe Z
    {PX, PZ, R, 300, 20},
    {PX, PZ, PY, 200, 500},
};

float WalkBottomMatrice[][5] = {
    {PX, PZ, PY, 500, 100}, // Patte par défaut
    {PX, PZ, R, 300, 40}, // Patte reculée sur l'axe Y
    {PX, PZ, R*2, 300, 40}, // Patte reculée sur l'axe Y
    {PX, PZ, PY, 500, 1000}, // Patte par défaut
};


void moveLegsSmoothly(int legIndices[], int numLegs, float targetX, float targetZ, float targetY, int duration) {
    float startX[numLegs], startZ[numLegs], startY[numLegs];

    for (int i = 0; i < numLegs; ++i) {
        startX[i] = CurrentPosition[legIndices[i]].x;
        startZ[i] = CurrentPosition[legIndices[i]].z;
        startY[i] = CurrentPosition[legIndices[i]].y;
    }

    unsigned long startTime = millis();
    unsigned long currentTime = startTime;

    while (currentTime - startTime < duration) {
        currentTime = millis();
        float progress = (float)(currentTime - startTime) / duration;

        for (int i = 0; i < numLegs; ++i) {
            float newX = startX[i] + (targetX - startX[i]) * progress;
            float newZ = startZ[i] + (targetZ - startZ[i]) * progress;
            float newY = startY[i] + (targetY - startY[i]) * progress;

            int* leg = legs[legIndices[i]];
            int address = (legIndices[i] < 3) ? 1 : 0;
          
            setLeg(newX, newZ, newY, 0, leg, address);
        }

        delay(1); // Petite pause pour assurer la fluidité du mouvement
    }

    // Mise à jour de la position finale
    for (int i = 0; i < numLegs; ++i) {
        setLeg(targetX, targetZ, targetY, 0, legs[legIndices[i]], (legIndices[i] < 3) ? 1 : 0);

        // Mise à jour de la position actuelle
        CurrentPosition[legIndices[i]].x = targetX;
        CurrentPosition[legIndices[i]].z = targetZ;
        CurrentPosition[legIndices[i]].y = targetY;
    }
}

void Walk() {
    for (int i = 0; i < sizeof(WalkUpMatrice) / sizeof(WalkUpMatrice[0]); ++i) {
        /*
        {
          0: LFL -> leg front left;
          1: LBL -> leg back left;
          2: LML -> leg middle left;
          3: LFR -> leg front right;
          4: LMR -> leg middle right;
          5: LBR -> leg back right;
        }
        */

        int legIndices[] = {0, 1, 4}; // Indices des pattes à déplacer en même temps
        int legIndicesBottom[] = {2, 3, 5};

        
        if(i == 0 || i == 3 || i == 4 || i == 5) {
            int num = (i == 0) ? 0 : i-2;
            moveLegsSmoothly(legIndicesBottom, 3, WalkBottomMatrice[i][0], WalkBottomMatrice[i][1], WalkBottomMatrice[i][2], WalkBottomMatrice[i][3]);
        }
        moveLegsSmoothly(legIndices, 3, WalkUpMatrice[i][0], WalkUpMatrice[i][1], WalkUpMatrice[i][2], WalkUpMatrice[i][3]);


        delay(WalkUpMatrice[i][4]);
    }
}
