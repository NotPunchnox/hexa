#include "math.h"

double radiansToDegrees(double radians) {
  return radians * (180.0 / M_PI);
}

float AlKashi(float a, float b, float c) {
  return radiansToDegrees(acos((a*a+b*b-c*c)/(2*a*b)));
}

float Racine(float a, float b) {
  return sqrt(a*a + b*b);
}
