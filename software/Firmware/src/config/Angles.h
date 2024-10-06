#ifndef ANGLES_H
#define ANGLES_H

#define MAX_LEG 6

struct LegPosition {
  float x;
  float z;
  float y;
};

extern LegPosition CurrentPosition[MAX_LEG];

#endif
