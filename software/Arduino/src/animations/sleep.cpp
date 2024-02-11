#include "../config/config.h"
#include "../functions/Algo.h"
#include "../functions/Servo.h"
#include <Arduino.h>

// Matrice de sleep_animation (x, z, y, ms) => hauteur, extension, rotation coxa en cm, temps du mouvement.
float sleep_animation[][5] = {
  {9, 10, 0, 500, 1000},
  {9, 5, 0, 500, 1},
  {9, 1, 0, 50, 1},
  {9, -3, 0, 500, 1},
  {9, -3, 0, 1000, 1000},
  {9, -5, 0, 1000, 1000},
};

void Sleep() {
  for (int i = 0; i < sizeof(sleep_animation) / sizeof(sleep_animation[0]); ++i) {
    LegAngles anglesFrontRight = Algo(sleep_animation[i][0], sleep_animation[i][1], sleep_animation[i][2], sleep_animation[i][3]);
    
    float AngleTibiaFR = anglesFrontRight.AngleTibia;
    float AngleFemurFR = anglesFrontRight.AngleFemur;
    float AngleCoxaFR = anglesFrontRight.AngleCoxa;

    //Pattes droite
    int servoChannelsFrontRight[] = {T_front_right, F_front_right, C_front_right};//front leg
    int servoChannelsMiddleRight[] = {T_mid_right, F_mid_right, C_mid_right};//middle leg
    float anglesR[] = {AngleTibiaFR, 180 - AngleFemurFR, AngleCoxaFR};
    setServo(servoChannelsFrontRight, 3, anglesR);
    setServo(servoChannelsMiddleRight, 3, anglesR);

    //Patte avant gauche
    
    LegAngles AFL = Algo(sleep_animation[i][0], sleep_animation[i][1], -sleep_animation[i][2], sleep_animation[i][3]);
    
    int servoChannelsFrontLeft[] = {T_front_left, F_front_left, C_front_left};
    int servoChannelsMiddleLeft[] = {T_mid_left, F_mid_left, C_mid_left};
    float anglesL[] = {90, AFL.AngleFemur, AFL.AngleCoxa};
    setServo(servoChannelsFrontLeft, 3, anglesL);
    setServo(servoChannelsMiddleLeft, 3, anglesL);
    

    delay(sleep_animation[i][4]);
  }
}
