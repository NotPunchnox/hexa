#include "servo.h"
#include "../config/config.h"
#include "../config/Angles.h"
#include <Adafruit_PWMServoDriver.h>
#include "../functions/Algo.h"

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x41);

#define MIN_PULSE_WIDTH 600
#define MAX_PULSE_WIDTH 2400
#define SERVO_FREQ 50

// Limites des angles des servomoteurs
#define MIN_ANGLE 0
#define MAX_ANGLE 180
#define MAX_LEG_PARTS 3

void Init() {
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);

  pwm2.begin();
  pwm2.setOscillatorFrequency(27000000);
  pwm2.setPWMFreq(SERVO_FREQ);

}

int pulseWidth(float angle) {
  int pulseWidthMicros = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  return pulseWidthMicros;
}
// Fonction pour déplacer une patte du robot aux coordonnées désirées
void setLeg(float x, float z, float y, int duree, int LEG[3], int address) {
    if (LEG == LFL || LEG == LBR) {
        y += 9;
        x -= 1;
    } else if (LEG == LFR || LEG == LBL) {
        y -= 9;
        x -= 1;
    }

    if (LEG == LML || LEG == LMR) {
      x = x +2;
      y = -y;
    }
    // Serial.print("\nx: ");
    // Serial.print(x);
    // Serial.print(" | y: ");
    // Serial.print(y);
    // Serial.print(" | z: ");
    // Serial.print(z);

    LegAngles res = Algo(x, z, y, duree);

    float angles[3];

    if (LEG == LML || LEG == LFL || LEG == LBL) { // Si c'est une patte gauche, il faut inverser les angles tibia et fémur
        angles[0] = res.AngleTibia;
        angles[1] = 180 - res.AngleFemur;
        angles[2] = res.AngleCoxa;
    } else {
        angles[0] = 180 - res.AngleTibia;
        angles[1] = res.AngleFemur;
        angles[2] = res.AngleCoxa;
    }

    if(angles[0] > 180) {
      angles[0] = 180;
    } else if (angles[0] < 0) {
      angles[0] = 1;
    }
    if(angles[1] > 180) {
      angles[1] = 180;
    } else if (angles[1] < 0) {
      angles[1] = 1;
    }
    if(angles[2] > 180) {
      angles[2] = 180;
    } else if (angles[1] < 0) {
      angles[2] = 1;
    }
    // Serial.println(" ");
    // Serial.println("\nAngle coxa: ");
    // Serial.print(angles[2]);
    // Serial.println("\nAngle femur: ");
    // Serial.print(angles[1]);
    // Serial.println("\nAngle tibia: ");
    // Serial.print(angles[0]);

    setServo(LEG, 3, angles, address, duree);
}
void moveLegsSmoothly(int legIndices[], int numLegs, float targetX, float targetZ, float targetY, int duration) {
    float startX[numLegs], startZ[numLegs], startY[numLegs];

    for (int i = 0; i < numLegs; ++i) {
        startX[i] = CurrentPosition[legIndices[i]].x;
        startZ[i] = CurrentPosition[legIndices[i]].z;
        startY[i] = CurrentPosition[legIndices[i]].y;
    }

    unsigned long startTime = millis();
    unsigned long currentTime = startTime;

    while (currentTime - startTime < duration) {
        currentTime = millis();
        float progress = (float)(currentTime - startTime) / duration;

        for (int i = 0; i < numLegs; ++i) {
            float newZ = startZ[i] + (targetZ - startZ[i]) * progress;
            float newX = startX[i] + (targetX - startX[i]) * progress;
            float newY = startY[i] + (targetY - startY[i]) * progress;

            int* leg = legs[legIndices[i]];
            int address = (legIndices[i] < 3) ? 1 : 0;
          
            setLeg(newX, newZ, newY, 0, leg, address);
        }

        delay(1); // Petite pause pour assurer la fluidité du mouvement
    }

    // Mise à jour de la position finale
    for (int i = 0; i < numLegs; ++i) {
        setLeg(targetX, targetZ, targetY, 0, legs[legIndices[i]], (legIndices[i] < 3) ? 1 : 0);
        
        // Mise à jour de la position actuelle
        CurrentPosition[legIndices[i]].z = targetZ;
        CurrentPosition[legIndices[i]].x = targetX;
        CurrentPosition[legIndices[i]].y = targetY;
    }
}

// Fonction pour déplacer les pattes progressivement avec des coordonnées différentes par pattes
void moveLegsMatrices(int legIndices[], float targetX[], float targetZ[], float targetY[], int numLegs, int duration) {
    float startX[numLegs], startZ[numLegs], startY[numLegs];

    for (int i = 0; i < numLegs; ++i) {
        startX[i] = CurrentPosition[legIndices[i]].x;
        startZ[i] = CurrentPosition[legIndices[i]].z;
        startY[i] = CurrentPosition[legIndices[i]].y;
    }

    unsigned long startTime = millis();
    unsigned long currentTime = startTime;

    while (currentTime - startTime < duration) {
        currentTime = millis();
        float progress = (float)(currentTime - startTime) / duration;

        for (int i = 0; i < numLegs; ++i) {
            float newX = startX[i] + (targetX[i] - startX[i]) * progress;
            float newZ = startZ[i] + (targetZ[i] - startZ[i]) * progress;
            float newY = startY[i] + (targetY[i] - startY[i]) * progress;

            int* leg = legs[legIndices[i]];
            int address = (legIndices[i] < 3) ? 1 : 0;

            setLeg(newX, newZ, newY, 0, leg, address);
        }

        delay(1);
    }

    // Mise à jour de la position finale
    for (int i = 0; i < numLegs; ++i) {
        setLeg(targetX[i], targetZ[i], targetY[i], 0, legs[legIndices[i]], (legIndices[i] < 3) ? 1 : 0);
        CurrentPosition[legIndices[i]].x = targetX[i];
        CurrentPosition[legIndices[i]].z = targetZ[i];
        CurrentPosition[legIndices[i]].y = targetY[i];
    }
}

void setServo(int servoChannels[], int numChannels, float angles[], int address, int duree) {
    for (int i = 0; i < numChannels; ++i) {
        if (address == 1) {
          pwm2.writeMicroseconds(servoChannels[i], pulseWidth(angles[i]));
        } else {
          pwm.writeMicroseconds(servoChannels[i], pulseWidth(angles[i]));
        }
    }
}
