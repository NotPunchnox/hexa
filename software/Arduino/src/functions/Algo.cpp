#include "Algo.h"
#include "HexaMath.h"
#include "../config/config.h"
#include "math.h"

LegAngles Algo(float x, float z, float y, int duree) {
    LegAngles angles;
    /*
    x = taille de la patte; z = hauteur; y = rotation (cm);
    duree = durée du mouvement en millisecondes;
    */
    float hyp = Racine(x, z);
    float A1 = AlKashi(hyp, z, x);
    float A2 = AlKashi(hyp, femur, tibia);
    float TPatte = Racine(z, x + coxa);

    angles.AngleFemur = A1 + A2;
    float A3 = AlKashi(tibia, femur, hyp);

    angles.AngleCoxa = 90 + radiansToDegrees(asin(y / TPatte));
    angles.AngleTibia = A3;

    return angles;
}
