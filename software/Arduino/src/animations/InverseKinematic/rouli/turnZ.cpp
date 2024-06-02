#include "./rouli.h"

void TurnZ(String sens, float speed) {
    float s = 100 * speed;
    for (int i = 0; i < 3; ++i) {
        if (sens == "horaire") {
            // Séquence pour la rotation dans le sens horaire
            Rouli(s, 3.0, 0.0, 1.0, 0.0);
            Rouli(s, 1.0, 0.0, 3.0, 0.0);
            Rouli(s, 0.0, 1.0, 3.0, 0.0);
            Rouli(s, 0.0, 3.0, 1.0, 0.0);
            Rouli(s, 0.0, 3.0, 0.0, 1.0);
            Rouli(s, 0.0, 1.0, 0.0, 3.0);
            Rouli(s, 1.0, 0.0, 0.0, 3.0);
            Rouli(s, 3.0, 0.0, 0.0, 1.0);
        } else if (sens == "anti-horaire") {
            // Séquence pour la rotation dans le sens anti-horaire
            Rouli(s, 3.0, 0.0, 0.0, 1.0);
            Rouli(s, 1.0, 0.0, 0.0, 3.0);
            Rouli(s, 0.0, 1.0, 0.0, 3.0);
            Rouli(s, 0.0, 3.0, 0.0, 1.0);
            Rouli(s, 0.0, 3.0, 1.0, 0.0);
            Rouli(s, 0.0, 1.0, 3.0, 0.0);
            Rouli(s, 1.0, 0.0, 3.0, 0.0);
            Rouli(s, 3.0, 0.0, 1.0, 0.0);
        }
    }
}
