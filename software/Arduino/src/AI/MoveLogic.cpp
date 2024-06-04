#include "../animations/InverseKinematic/poses/Top.h"
#include "../animations/InverseKinematic/rouli/rouli.h"
#include "../animations/InverseKinematic/rouli/turnZ.h"
#include "../animations/InverseKinematic/turn/turn.h"
#include "../animations/default/default.h"
#include "../animations/sleep/sleep.h"
#include "../animations/up/Up.h"
#include "../animations/walk/walk.h"

// Fonction pour diviser une chaîne de caractères en morceaux basés sur un délimiteur
void splitString(String str, char delimiter, String* resultArray, int maxParts) {
    int currentIndex = 0;
    int arrayIndex = 0;

    while (arrayIndex < maxParts && currentIndex != -1) {
        int nextIndex = str.indexOf(delimiter, currentIndex);

        if (nextIndex == -1) {
            resultArray[arrayIndex++] = str.substring(currentIndex);
        } else {
            resultArray[arrayIndex++] = str.substring(currentIndex, nextIndex);
        }

        currentIndex = nextIndex + 1;
    }
}

void IA_Movements(String response) {
    if(response.indexOf("ChangeTop_") != -1) {
        String parts[3]; // Tableau pour stocker les morceaux de la chaîne de caractères
        splitString(response, '_', parts, 3);

        int speed = parts[1].toFloat();
        int cm = parts[2].toFloat();

        ChangeTop(speed, cm);
    } else if (response.indexOf("TurnZ_") != -1) {
        String parts[3];
        splitString(response, '_', parts, 3);

        int speed = parts[1].toFloat();
        String side = parts[2];

        if(side == "horaire" || side == "anti-horaire") {
            TurnZ(side, speed, 1);
        }
    } else if (response.indexOf("Sleep_") != -1) {
        String parts[3];
        splitString(response, '_', parts, 3);

        float speed = parts[1].toFloat();

        Sleep(speed);
        
    } else if (response.indexOf("Walk_") != -1) {
        String parts[3];
        splitString(response, '_', parts, 3);

        float speed = parts[1].toFloat();
        int cycles = parts[2].toInt();

        for (int i = 0; i < cycles; ++i) {
            Walk(speed);
        }
    }
}
