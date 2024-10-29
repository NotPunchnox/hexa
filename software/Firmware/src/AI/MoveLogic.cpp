#include <Arduino.h>
#include <QueueArray.h>
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

        isAnimated = true;
        ChangeTop(speed, cm);
        isAnimated = false;
    } else if (response.indexOf("TurnZ_") != -1) {
        String parts[3];
        splitString(response, '_', parts, 3);

        int speed = parts[1].toFloat();
        String side = parts[2];

        if (side == "horaire" || side == "anti-horaire") {
            isAnimated = true;
            TurnZ(side, speed, 1);
            isAnimated = false;
        }
    } else if (response.indexOf("Sleep_") != -1) {
        String parts[3];
        splitString(response, '_', parts, 3);

        float speed = parts[1].toFloat();
        isAnimated = true;
        Sleep(speed);
        isAnimated = false;
    } else if (response.indexOf("StartWalk_") != -1) {//StartWalk_speed_X_Y
        String parts[4];
        splitString(response, '_', parts, 4);

        float speed = parts[1].toFloat();
        int X = parts[2].toFloat();
        int Y = parts[3].toFloat();
        isAnimated = true;
        startWalking(X, Y, speed);
        isAnimated = false;
    }  else if (response.indexOf("StopWalk") != -1) {//StopWalk
        Walk(4, 0, 0);
        isWalking = false;
    } else if (response.indexOf("Walk_") != -1) {
        // Log de la réponse initiale
        Serial.println("Commande reçue: " + response);

        String parts[5];
        splitString(response, '_', parts, 5);

        // Log des parties après le split
        Serial.println("Parts après le split:");
        for (int i = 0; i < 5; ++i) {
            Serial.println("Part " + String(i) + ": " + parts[i]);
        }

        float speed = parts[1].toFloat();
        int X = parts[2].toInt();
        int Y = parts[3].toInt();
        int cycles = parts[4].toInt();

        // Log des valeurs converties
        Serial.println("Vitesse: " + String(speed));
        Serial.println("X: " + String(X));
        Serial.println("Y: " + String(Y));
        Serial.println("Cycles: " + String(cycles));

        for (int i = 0; i < cycles; ++i) {
            Serial.println("Exécution de la boucle: " + String(i + 1));
            isAnimated = true;
            Walk(speed, X, Y);
            isAnimated = false;
        }

        Serial.println("Commande Walk terminée");
    } else if (response.indexOf("StartTurn_") != -1) {//StartTurn_speed_side_rayon
        String parts[4];
        splitString(response, '_', parts, 4);

        float speed = parts[1].toFloat();
        String side = parts[2];
        float rayon = parts[3].toFloat();
        
        isAnimated = true;
        startTurning(side, speed, rayon);
        isAnimated = false;
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

            isAnimated = true;
            Turn(side, speed, cycles, r);
            isAnimated = false;
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

        isAnimated = true;
        Rouli(speed, top, bottom, left, right);
        isAnimated = false;
    }else if (response.indexOf("ChangeY_") != -1) {
        String parts[3];
        splitString(response, '_', parts, 3);

        int speed = parts[1].toFloat();
        int cm = parts[2].toFloat();

        isAnimated = true;
        ChangeY(speed, cm);
        isAnimated = false;
    }else if (response.indexOf("ChangeX_") != -1) {
        String parts[3];
        splitString(response, '_', parts, 3);

        int speed = parts[1].toFloat();
        int cm = parts[2].toFloat();

        isAnimated = true;
        ChangeX(speed, cm);
        isAnimated = false;
    } else if (response.indexOf("ChangeXY_") != -1) {
        String parts[4];
        splitString(response, '_', parts, 4);

        if (parts[1].length() > 0 && parts[2].length() > 0 && parts[3].length() > 0) {
            float speed = parts[1].toFloat();
            float X = parts[2].toFloat();
            int Y = parts[3].toInt();

            isAnimated = true;
            ChangeXY(speed, X, Y);
            isAnimated = false;
        } else {
            Serial.println("Erreur : Format de commande incorrect.");
        }
    } else if (response.indexOf("Custom_") != -1) {
        String parts[4]; 
        splitString(response, '_', parts, 4);

        Serial.println("Split Parts:");
        for(int i = 0; i < 4; i++) {
            Serial.print("Part["); Serial.print(i); Serial.print("]: ");
            Serial.println(parts[i]);
        }

        if (parts[1].length() > 0 && parts[2].length() > 0 && parts[3].length() > 0) {
            String matrixString = parts[1] + "_" + parts[2];
            float speed = parts[2].toFloat();

            float animationMatrix[6][1][3];
            parseCustomMatrix(matrixString, animationMatrix);

            isAnimated = true;
            Custom(animationMatrix, speed);
            isAnimated = false;
        } else {
            Serial.println("Erreur : Format de commande incorrect.");
        }
    } else if (response.indexOf("Attack_") != -1) {
        String parts[3];
        splitString(response, '_', parts, 2);

        int speed = parts[1].toFloat();

        isAnimated = true;
        AttackMove(speed);
        isAnimated = false;
    } else if (response.indexOf("Jump_") != -1) {
        String parts[3];
        splitString(response, '_', parts, 2);

        int speed = parts[1].toFloat();

        isAnimated = true;
        Jump(speed);
        isAnimated = false;
    } 

}


QueueArray<String> commandQueue;

void enqueueCommand(const String& command) {
    commandQueue.enqueue(command);
}

// Fonction pour diviser et ajouter plusieurs commandes dans la file
void parseAndEnqueueCommands(const String& commands) {
    String command;
    for (int i = 0; i < commands.length(); i++) {
        char currentChar = commands.charAt(i);
        if (currentChar == ';') {
            if (command.length() > 0) {
                command.trim(); // Applique trim pour enlever les espaces
                enqueueCommand(command); // Ajoute la commande sans trim()
                command = "";
            }
        } else {
            command += currentChar;
        }
    }

    if (command.length() > 0) {
        command.trim(); // Applique trim pour la dernière commande
        enqueueCommand(command);
    }
}

// Fonction pour traiter la prochaine commande
void processNextCommand() {
    if (!isAnimated && !commandQueue.isEmpty()) {
        String command = commandQueue.dequeue();
        IA_Movements(command);  // Appelle IA_Movements
        isAnimated = true;
    }
}