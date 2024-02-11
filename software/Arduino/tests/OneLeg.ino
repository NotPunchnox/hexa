#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

#define SERVOMIN  150
#define SERVOMAX  600
#define SERVO_FREQ 50

/*
void setServo(uint8_t n, int degrees) {
  // Convertissez les degrés en microsecondes
  int pulse = map(degrees, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(n, 0, pulse);
}

double radiansToDegrees(double radians) {
  return radians * (180.0 / M_PI);
}

void Algo(float x, float y, float coxa, float femur, float tibia, int tibiaServoChannel, int femurServoChannel, int coxaServoChannel) {
  float hypotenuse = sqrt(y * y + x * x);
  float a1 = acos((y*y + hypotenuse*hypotenuse - x*x)/(2*y*hypotenuse));
  float a2 = acos((femur*femur+hypotenuse*hypotenuse-tibia*tibia)/(2*femur*hypotenuse));
  float a3 = acos((femur*femur+tibia*tibia-hypotenuse*hypotenuse)/(2*femur*tibia));

  float angleFemur = 180 - (radiansToDegrees(a1) + radiansToDegrees(a2));
  float angleTibia = radiansToDegrees(a3);

  Serial.print("Position (x, y): (");
  Serial.print(x);
  Serial.print(", ");
  Serial.print(y);
  Serial.println(")");

  Serial.print("Longueur de l'hypotenuse: ");
  Serial.print(hypotenuse);
  Serial.println(" cm");

  Serial.print("Angle du Femur: ");
  Serial.print(angleFemur);
  Serial.println(" degrés");

  Serial.print("Angle du Tibia: ");
  Serial.print(angleTibia);
  Serial.println(" degrés");

  setServo(tibiaServoChannel, angleTibia);
  setServo(femurServoChannel, angleFemur);
}

// Matrice de positions (x, y, ms)
float positions[][3] = {
  {12, 10, 7},
  {10, -3, 1000},
  {12, -8, 60},
  {10, -3, 70},
  {12, -8, 30},
  {10, -3, 70},
  {12, -8, 30},
  {12, 9, 120},
  {12, 8, 120},
  {12, 7, 120},
  {12, 6, 120},
  {12, 5, 120},
  {12, 4, 120},
  {12, 3, 120},
  // Ajoute d'autres positions si nécessaire
};

void mouvementFluide(float x1, float y1, float x2, float y2, float coxa, float femur, float tibia, int duree, int tibiaServoChannel, int femurServoChannel, int coxaServoChannel) {
  int nombreIntervalles = 1; // Fps par mouvement
  float deltaX = (x2 - x1) / nombreIntervalles;
  float deltaY = (y2 - y1) / nombreIntervalles;

  for (int i = 0; i <= nombreIntervalles; ++i) {
    float xIntermediaire = x1 + i * deltaX;
    float yIntermediaire = y1 + i * deltaY;
    Algo(xIntermediaire, yIntermediaire, coxa, femur, tibia, tibiaServoChannel, femurServoChannel, coxaServoChannel);
    delay(duree / nombreIntervalles);
  }
}

void setup() {
  Serial.begin(9600);

  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);

  int tibiaServoChannel = 0;  // canal pour le servo du tibia
  int femurServoChannel = 1;  // canal pour le servo du femur
  int coxaServoChannel = 2;  // canal pour le servo du coxa

  for (int i = 0; i < sizeof(positions) / sizeof(positions[0]) - 1; ++i) {
    mouvementFluide(positions[i][0], positions[i][1], positions[i + 1][0], positions[i + 1][1], 5, 6.2, 14, positions[i][2], tibiaServoChannel, femurServoChannel, coxaServoChannel);
  }
}

void loop() {
  // Boucle vide, le programme ne s'exécute que dans la fonction setup()
}
*/