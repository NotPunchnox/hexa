#include "../config/config.h"
#include "../functions/Algo.h"
#include <Arduino.h>
#include "../functions/Servo.h"

// Matrice de positions (x, z, y, ms) => hauteur, extension, rotation coxa en cm, temps du mouvement.
float positions[][5] = {
  {9, 10, 0, 1, 1000},
  {9, 1, 0, 700, 700},
  {9, 1, -10, 700, 700},
  {9, 10, -10, 700, 700},
  {9, 10, 0, 700, 10},
};

void Walk() {
    for (int i = 0; i < sizeof(positions) / sizeof(positions[0]); ++i) {
    LegAngles anglesFrontRight = Algo(positions[i][0], positions[i][1], positions[i][2], positions[i][3]);
    
    float AngleTibiaFR = anglesFrontRight.AngleTibia;
    float AngleFemurFR = anglesFrontRight.AngleFemur;
    float AngleCoxaFR = anglesFrontRight.AngleCoxa;

    //Patte avant droite
    int servoChannelsFrontRight[] = {T_front_right, F_front_right, C_front_right};
    float anglesFR[] = {AngleTibiaFR, AngleFemurFR, AngleCoxaFR};
    setServo(servoChannelsFrontRight, 3, anglesFR);

    /*
    //Patte avant gauche
    float AngleTibiaFL, AngleFemurFL, AngleCoxaFL; // Calculer ces angles
    int servoChannelsFrontLeft[] = {T_front_left, F_front_left, C_front_left};
    float anglesFrontLeft[] = {AngleTibiaFL, 180-AngleFemurFL, 90-AngleCoxaFL};
    setServo(servoChannelsFrontLeft, 3, anglesFrontLeft);
    */

    delay(positions[i][4]);
  }
}
