#ifndef WALK_H
#define WALK_H

#include <Arduino.h>
#include "../../functions/Algo.h"

void Walk();
void setLeg(float x, float z, float y, int duree, int LEG[3], int address);
void moveLegsSmoothly(int legIndices[], int numLegs, float targetX, float targetZ, float targetY, int duration);

#endif
