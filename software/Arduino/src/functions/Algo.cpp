#include "Algo.h"
#include "HexaMath.h"
#include "../config/config.h"
#include "math.h"
#include "Arduino.h"

LegAngles Algo(float x, float z, float y, int duree) {
    LegAngles angles;
    /*
    x = taille de la patte; z = hauteur; y = rotation (cm);
    duree = durée du mouvement en millisecondes;
    */
    float TPatte = Racine(y, x);
    Serial.println("\nTpatte: ");
    Serial.print(TPatte);
    float hyp = Racine(x, TPatte-coxa);
    Serial.println("\nHypotenuse: ");
    Serial.println(hyp);
    float A1 = radiansToDegrees(atan((TPatte-coxa)/z));
    float A2 = AlKashi(femur, hyp, tibia);

    angles.AngleFemur = A1 + A2;
    Serial.println(angles.AngleFemur);
    float A3 = AlKashi(tibia, femur, hyp);
    Serial.println("\nTibia: ");
    Serial.println(A3);

    angles.AngleCoxa = 90 + radiansToDegrees(atan(y / x));
    angles.AngleTibia = A3;

    return angles;
}
