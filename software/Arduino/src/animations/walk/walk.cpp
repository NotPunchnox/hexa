#include "../../config/config.h"
#include "../../functions/Algo.h"
#include <Arduino.h>
#include "../../functions/Servo.h"

// Diamètre cercle virtuel: 4cm ( extension de la patte lors de la marche ).
// Elévation de la patte de 5cm lors de la marche.
// Matrice de positions (x, z, y, ms, attente) => hauteur, extension, rotation coxa en cm, temps du mouvement, temps d'attente une fois le mouvement fait.
float positions[][5] = {
  {PX, PZ, PY, 0, 1000},
  {PX, PZ - 5, PY, 0, 100},
  {PX, PZ -5, PY, 0, 1000},//On soulève la patte de 5cm par rapport à la position défaut
  {PX, PZ -5, PY + 4, 0, 1000},// on avance la patte de 4cm sur l'axe Y
  {PX, PZ, PY +4, 0, 1000},
  {PX, PZ, PY, 0, 1000},
};

void Walk() {
    for (int i = 0; i < sizeof(positions) / sizeof(positions[0]); ++i) {

    delay(positions[i][4]);
  }
}
