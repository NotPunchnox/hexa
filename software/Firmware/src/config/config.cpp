// /!\ ATTENTION: les pattes ont comme adresse le premier moteur ( ex: LegFrontLeft = 5 ), si vous changez la configuration veuillez ne pas dubliquer une adresse.

int T_front_right = 12;
int F_front_right = 10;
int C_front_right = 7;

int T_mid_right = 4;
int F_mid_right = 5;
int C_mid_right = 6;

int T_back_right = 0;
int F_back_right = 1;
int C_back_right = 2;

int T_front_left = 5;//2
int F_front_left = 6;//2
int C_front_left = 7;//2

int T_mid_left = 9;//2
int F_mid_left = 10;//2
int C_mid_left = 11;//2

int T_back_left = 14;//2
int F_back_left = 13;//2
int C_back_left = 12;//2


float coxa = 5.0;
float femur = 6.3;
float tibia = 13.0;

//LFR = lEG FRONT RIGHT
//LMR = LEG MIDDLE RIGHT
//LBR = LEG BACK RIGHT

//LFL = lEG FRONT LEFT
//LML = LEG MIDDLE LEFT
//LBL = LEG BACK LEFT

int LFR[] = {T_front_right, F_front_right, C_front_right};
int LMR[] = {T_mid_right, F_mid_right, C_mid_right};
int LBR[] = {T_back_right, F_back_right, C_back_right};//address: 2

int LFL[] = {T_front_left, F_front_left, C_front_left};
int LML[] = {T_mid_left, F_mid_left, C_mid_left};//address: 2
int LBL[] = {T_back_left, F_back_left, C_back_left};//address: 2


float default_pose[3] = {10, 9, 0};

float PX = default_pose[0];
float PZ = default_pose[1];
float PY = default_pose[2];

int* legs[] = {LFL, LML, LBL, LFR, LMR, LBR};