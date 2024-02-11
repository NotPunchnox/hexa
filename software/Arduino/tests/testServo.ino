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

  tibiaServo.writeMicroseconds(map(angleTibia, 0, 180, 1000, 2000));
  femurServo.writeMicroseconds(map(angleFemur, 0, 180, 1000, 2000));
}


void setup() {
  Serial.begin(9600);
  tibiaServo.attach(8);
  femurServo.attach(9);

  tibiaServo.write(170);
  femurServo.writeMicroseconds(map(3.78, 0, 180, 0, 2000));

  //Algo(1, 8, 5, 6, 14);

}

void loop() {
  // Boucle vide, le programme ne s'exécute que dans la fonction setup()
}
