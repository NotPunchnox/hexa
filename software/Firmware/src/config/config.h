#ifndef CONFIG_H
#define CONFIG_H

// Patte avant droite
extern int F_front_right;
extern int T_front_right;
extern int C_front_right;
// Patte millieu droite
extern int F_mid_right;
extern int T_mid_right;
extern int C_mid_right;
// Patte millieu droite
extern int F_back_right;
extern int T_back_right;
extern int C_back_right;


// Patte avant gauche
extern int F_front_left;
extern int T_front_left;
extern int C_front_left;
// Patte millieu gauche
extern int F_mid_left;
extern int T_mid_left;
extern int C_mid_left;
// Patte arrière gauche
extern int F_back_left;
extern int T_back_left;
extern int C_back_left;


extern float coxa;
extern float femur;
extern float tibia;


//LEGS
extern int LFR[3];
extern int LMR[3];
extern int LFL[3];
extern int LML[3];
extern int LBL[3];
extern int LBR[3];

extern float default_pose[3];
extern float PX;
extern float PZ;
extern float PY;

extern int* legs[];

#endif
