#include "../animations/InverseKinematic/poses/Top.h"
#include "../animations/InverseKinematic/rouli/rouli.h"
#include "../animations/InverseKinematic/rouli/turnZ.h"
#include "../animations/InverseKinematic/turn/turn.h"
#include "../animations/default/default.h"
#include "../animations/sleep/sleep.h"
#include "../animations/up/Up.h"
#include "../animations/walk/walk.h"
#include "../animations/custom/custom.h"
#include "../animations/fun/fun.h"
#include "../router/Walking.h"

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
void parseCustomMatrix(String matrixString, float animationMatrix[6][1][3]) {
    String positions[6];
    splitString(matrixString, ';', positions, 6); // Diviser en positions pour chaque patte

    for (int leg = 0; leg < 6; ++leg) {
        String coords[3];
        splitString(positions[leg], ',', coords, 3); // Diviser en coordonnées x, z, y
        animationMatrix[leg][0][0] = coords[0].toFloat(); // x
        animationMatrix[leg][0][1] = coords[1].toFloat(); // z
        animationMatrix[leg][0][2] = coords[2].toFloat(); // y
    }
}

void IA_Movements(String response) {
    if (response.indexOf("ChangeTop_") != -1) {
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

        if (side == "horaire" || side == "anti-horaire") {
            TurnZ(side, speed, 1);
        }
    } else if (response.indexOf("Sleep_") != -1) {
        String parts[3];
        splitString(response, '_', parts, 3);

        float speed = parts[1].toFloat();

        Sleep(speed);
        
    } else if (response.indexOf("StartWalk_") != -1) {//StartWalk_speed_X_Y
        String parts[4];
        splitString(response, '_', parts, 4);

        float speed = parts[1].toFloat();
        int X = parts[2].toFloat();
        int Y = parts[3].toFloat();
        
        startWalking(X, Y, speed);
    }  else if (response.indexOf("StopWalk") != -1) {//StopWalk
        Walk(3, 0, 0);
        isWalking = false;
        Serial.println("Animation:startWalk:terminé");
    } else if (response.indexOf("Walk_") != -1) {

        String parts[5];
        splitString(response, '_', parts, 5);

        float speed = parts[1].toFloat();
        int X = parts[2].toInt();
        int Y = parts[3].toInt();
        int cycles = parts[4].toInt();

        // // Log des valeurs converties
        // Serial.println("Vitesse: " + String(speed));
        // Serial.println("X: " + String(X));
        // Serial.println("Y: " + String(Y));
        // Serial.println("Cycles: " + String(cycles));

        for (int i = 0; i < cycles; ++i) {
            Serial.println("Walk:" + String(i) + ":" + String(cycles));
            Walk(speed, X, Y);
        }
        Serial.println("Animation:Walk:terminé");
    } else if (response.indexOf("StartTurn_") != -1) {//StartTurn_speed_side_rayon
        String parts[4];
        splitString(response, '_', parts, 4);

        float speed = parts[1].toFloat();
        String side = parts[2];
        float rayon = parts[3].toFloat();
        
        startTurning(side, speed, rayon);
    }  else if (response.indexOf("StopTurn") != -1) {//StopWalk
        Turn("left", 2, 1, 0);
        isTurning = false;
    } else if (response.indexOf("Turn_") != -1) {
        String parts[5];
        splitString(response, '_', parts, 5);
        float r = 1.5;

        if (parts[1].length() > 0 && parts[2].length() > 0 && parts[3].length() > 0) {
            float speed = parts[1].toFloat();
            String side = parts[2];
            int cycles = parts[3].toInt();
            if(parts[4].length() > 0) {
                r = parts[4].toFloat();
            }

            Turn(side, speed, cycles, r);
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
    }else if (response.indexOf("ChangeY_") != -1) {
        String parts[3];
        splitString(response, '_', parts, 3);

        int speed = parts[1].toFloat();
        int cm = parts[2].toFloat();

        ChangeY(speed, cm);
    }else if (response.indexOf("ChangeX_") != -1) {
        String parts[3];
        splitString(response, '_', parts, 3);

        int speed = parts[1].toFloat();
        int cm = parts[2].toFloat();

        ChangeX(speed, cm);
    } else if (response.indexOf("ChangeXY_") != -1) {
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
    } else if (response.indexOf("Custom_") != -1) {
        String parts[4]; 
        splitString(response, '_', parts, 4);

        for(int i = 0; i < 4; i++) {
            Serial.print("Part["); Serial.print(i); Serial.print("]: ");
            Serial.println(parts[i]);
        }

        if (parts[1].length() > 0 && parts[2].length() > 0 && parts[3].length() > 0) {
            String matrixString = parts[1] + "_" + parts[2];
            float speed = parts[2].toFloat();

            float animationMatrix[6][1][3];
            parseCustomMatrix(matrixString, animationMatrix);

            Custom(animationMatrix, speed);
        } else {
            Serial.println("Erreur : Format de commande incorrect.");
        }
    } else if (response.indexOf("Attack_") != -1) {
        String parts[3];
        splitString(response, '_', parts, 2);

        int speed = parts[1].toFloat();

        AttackMove(speed);
    } else if (response.indexOf("Jump_") != -1) {
        String parts[3];
        splitString(response, '_', parts, 2);

        int speed = parts[1].toFloat();

        Jump(speed);
    } 

}

