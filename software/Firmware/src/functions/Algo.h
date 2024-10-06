#ifndef ALGO_H
#define ALGO_H

#include "../config/config.h"

struct LegAngles {
    float AngleCoxa;
    float AngleFemur;
    float AngleTibia;
};

LegAngles Algo(float x, float z, float y, int duree);

#endif
