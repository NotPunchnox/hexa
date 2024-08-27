#ifndef WALKING_H
#define WALKING_H

extern float speed;
extern float X;
extern float Y;
extern bool isWalking;
extern unsigned long previousMillis;

void startWalking(float x, float y, float newspeed);

#endif
