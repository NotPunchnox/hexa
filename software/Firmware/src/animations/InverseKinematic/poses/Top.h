#ifndef TOP_H
#define TOP_H

#include "../../../functions/Algo.h"
#include "../../../config/config.h"
#include "../../../functions/servo.h"

void ChangeTop(float speed, float Top);
void ChangeY(float speed, float Y);
void ChangeX(float speed, float X);
void ChangeXY(float speed, float X, float Y);
void RouliXY(float speed, float X, float Y, float Top, float Bottom, float Left, float Right); 

#endif
