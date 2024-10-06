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
   //Calcul des valeurs manquantes ( Taille de la patte vu du ciel, et la taille de l'hypotenuse formé par le bout du coxa et çje bout de la patte )
    float TPatte = Racine(y, x);
    float hyp = Racine(z, TPatte-coxa);

    float A1 = radiansToDegrees(atan((TPatte-coxa)/z));
    float A2 = AlKashi(femur, hyp, tibia);
    angles.AngleFemur = A1 + A2;

    float A3 = AlKashi(tibia, femur, hyp);
   
    // Serial.println("\nTibia: ");
    // Serial.println(A3);

    angles.AngleCoxa = radiansToDegrees(atan(y / x)) + 90;
    angles.AngleTibia = A3;

    return angles;
}
