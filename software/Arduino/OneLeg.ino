#include <Servo.h>
#include <math.h>

Servo tibiaServo;
Servo femurServo;

double radiansToDegrees(double radians) {
  return radians * (180.0 / M_PI);
}

void Algo(float x, float y, float coxa, float femur, float tibia) {
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

  tibiaServo.write(angleTibia);
  femurServo.write(angleFemur);
/*
  tibiaServo.writeMicroseconds(map(angleTibia, 0, 180, 0, 2200));
  femurServo.writeMicroseconds(map(angleFemur, 0, 180, 0, 2200));
  */
}

// Matrice de positions (x, y, ms)
float positions[][3] = {
  {15, 15, 1500},
  {15, 10, 1500},
  {15, 5, 1500},
};

void mouvementFluide(float x1, float y1, float x2, float y2, float coxa, float femur, float tibia, int duree) {
  int nombreIntervalles = 7; // Fps par mouvement
  float deltaX = (x2 - x1) / nombreIntervalles;
  float deltaY = (y2 - y1) / nombreIntervalles;

  for (int i = 0; i <= nombreIntervalles; ++i) {
    float xIntermediaire = x1 + i * deltaX;
    float yIntermediaire = y1 + i * deltaY;
    Algo(xIntermediaire, yIntermediaire, coxa, femur, tibia);
    delay(duree / nombreIntervalles);
  }
}

void setup() {
  Serial.begin(9600);
  tibiaServo.attach(8);
  femurServo.attach(9);

  for (int i = 0; i < sizeof(positions) / sizeof(positions[0]) - 1; ++i) {
    mouvementFluide(positions[i][0], positions[i][1], positions[i + 1][0], positions[i + 1][1], 5, 6.2, 14, positions[i][2]);
  }
}

void loop() {
}
