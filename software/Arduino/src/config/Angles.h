#ifndef ANGLES_H
#define ANGLES_H

#define MAX_LEG 6

struct LegAngles {
  float coxa;
  float femur;
  float tibia;
};

extern LegAngles startAngles[MAX_LEG];

#endif
