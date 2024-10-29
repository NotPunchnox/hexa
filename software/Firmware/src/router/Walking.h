#ifndef WALKING_H
#define WALKING_H

extern float speed;
extern float rayon;
extern String Side;
extern float X;
extern float Y;

extern bool isAnimated;
extern bool isWalking;
extern bool isTurning;

extern unsigned long previousMillis;

void startWalking(float x, float y, float newspeed);
void startTurning(String side, float newspeed, float rayon);

#endif
