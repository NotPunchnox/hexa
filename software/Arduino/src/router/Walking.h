#ifndef WALKING_H
#define WALKING_H

extern float speed;
extern float X;
extern float Y;
extern bool isWalking;
extern unsigned long previousMillis;
extern int walkCycles;
extern int totalWalkCycles;

void startWalking(float x, float y, float newspeed);

#endif
