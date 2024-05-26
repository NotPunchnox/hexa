#ifndef WALK_H
#define WALK_H

#include <Arduino.h>
#include "../../functions/Algo.h"

void Walk(float speed);
void setLeg(float x, float z, float y, int duree, int LEG[3], int address);
void moveLegsSmoothly(int legIndices[], int numLegs, float targetX, float targetZ, float targetY, int duration);
void moveLegsSmoothlyWalk(int legIndices[], int legIndices2[], float targetX, float targetZ, float targetY, float targetX2, float targetZ2, float targetY2, int duration);

#endif
