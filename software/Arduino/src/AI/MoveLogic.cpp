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
void parseCustomMatrix(String matrixString, float animationMatrix[6][4][3]) {
    String steps[4];
    splitString(matrixString, '_', steps, 4);  // Diviser en étapes

    for (int step = 0; step < 4; ++step) {
        String positions[6];
        splitString(steps[step], ';', positions, 6); // Diviser en positions pour chaque patte

        for (int leg = 0; leg < 6; ++leg) {
            String coords[3];
            splitString(positions[leg], ',', coords, 3); // Diviser en coordonnées x, z, y
            animationMatrix[leg][step][0] = coords[0].toFloat(); // x
            animationMatrix[leg][step][1] = coords[1].toFloat(); // z
            animationMatrix[leg][step][2] = coords[2].toFloat(); // y
        }
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
    } else if(response.indexOf("Turn_") != -1) {
        String parts[4];
        splitString(response, '_', parts, 4);

        if (parts[1].length() > 0 && parts[2].length() > 0 && parts[3].length() > 0) {
            String side = parts[1];
            float speed = parts[2].toFloat();
            int cycles = parts[3].toInt();

            Turn(side, speed, cycles);
        } else {
            Serial.println("Erreur : Format de commande incorrect.");
        }
    } else if (response.indexOf("Rouli_") != -1) {
        String parts[6]; // Assurez-vous d'avoir suffisamment de parties
        splitString(response, '_', parts, 6); // Diviser en 6 parties

        float speed = parts[1].toFloat();
        float top = parts[2].toFloat();
        float bottom = parts[3].toFloat();
        float left = parts[4].toFloat();
        float right = parts[5].toFloat();

        Rouli(speed, top, bottom, left, right);
    }else if(response.indexOf("ChangeY_") != -1) {
        String parts[3];
        splitString(response, '_', parts, 3);

        int speed = parts[1].toFloat();
        int cm = parts[2].toFloat();

        ChangeY(speed, cm);
    }else if(response.indexOf("ChangeX_") != -1) {
        String parts[3];
        splitString(response, '_', parts, 3);

        int speed = parts[1].toFloat();
        int cm = parts[2].toFloat();

        ChangeX(speed, cm);
    } else if(response.indexOf("ChangeXY_") != -1) {
        String parts[4];
        splitString(response, '_', parts, 4);

        if (parts[1].length() > 0 && parts[2].length() > 0 && parts[3].length() > 0) {
            float speed = parts[1].toFloat();
            float X = parts[2].toFloat();
            int Y = parts[3].toInt();

            ChangeXY(speed, X, Y);
        } else {
            Serial.println("Erreur : Format de commande incorrect.");
        }
    } else if(response.indexOf("Custom_") != -1) {
        String parts[4]; 
        splitString(response, '_', parts, 4);

        if (parts[0] == "Custom" && parts[3].length() > 0) {
            String matrixString = parts[1] + "_" + parts[2];
            float speed = parts[3].toFloat();
            int cycles = parts[4].toInt();

            float animationMatrix[6][4][3];
            parseCustomMatrix(matrixString, animationMatrix);

            Custom(animationMatrix, 4, speed, cycles);
        } else {
            Serial.println("Erreur : Format de commande incorrect.");
        }
    }
}

