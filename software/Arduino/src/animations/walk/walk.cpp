#include "../../config/config.h"
#include "../../functions/Algo.h"
#include <Arduino.h>
#include "../../functions/Servo.h"

// Matrice de positions (x, z, y, ms, attente) => hauteur, extension, rotation coxa en cm, temps du mouvement, temps d'attente une fois le mouvement fait.
float positions[][5] = {
  /*{9, 10, 0, 1, 1000},
  {9, 1, 0, 700, 700},
  {9, 1, -10, 700, 700},
  {9, 10, -10, 700, 700},
  {9, 10, 0, 700, 10},*/

  {9, 10, 0, 1, 1000},
  {15.5, -1, 0, 1, 700},
  {15.5, -1, 10, 1, 700},
  {15.5, 10, 10, 1, 700},
  {9, 10, -10, 1, 1000},
  {9, 10, 0, 1, 1000},
};

void Walk() {
    for (int i = 0; i < sizeof(positions) / sizeof(positions[0]); ++i) {
      /*
    LegAngles anglesFrontRight = Algo(positions[i][0], positions[i][1], positions[i][2], positions[i][3]);
    
    float AngleTibiaFR = anglesFrontRight.AngleTibia;
    float AngleFemurFR = anglesFrontRight.AngleFemur;
    float AngleCoxaFR = anglesFrontRight.AngleCoxa;

    //Patte avant droite
    int servoChannelsFrontRight[] = {T_front_right, F_front_right, C_front_right};
    float anglesFR[] = {AngleTibiaFR, 180 - AngleFemurFR, AngleCoxaFR};
    setServo(servoChannelsFrontRight, 3, anglesFR);

    //Patte avant gauche
    LegAngles AFL = Algo(positions[i][0], positions[i][1], -positions[i][2], positions[i][3]);
    
    int servoChannelsFrontLeft[] = {T_front_left, F_front_left, C_front_left};
    float anglesFL[] = {180-AFL.AngleTibia, AFL.AngleFemur, AFL.AngleCoxa};
    setServo(servoChannelsFrontLeft, 3, anglesFL);
    */

    delay(positions[i][4]);
  }
}
