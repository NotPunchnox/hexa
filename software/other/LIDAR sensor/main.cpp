#include <iostream>
#include <wiringPiI2C.h>
#include <unistd.h>
#include <SerialStream.h>
using namespace LibSerial;

const int VL53L0X_ADDRESS = 0x29;

const int MAP_WIDTH = 10;
const int MAP_HEIGHT = 10;
int gridMap[MAP_HEIGHT][MAP_WIDTH] = {0};
int posX = MAP_WIDTH / 2, posY = MAP_HEIGHT / 2;
int orientation = 0; // 0=North, 1=East, 2=South, 3=West

SerialStream serialPort;

// Init VL53L0X sensor
int initVL53L0X() {
    int fd = wiringPiI2CSetup(VL53L0X_ADDRESS);
    if (fd == -1) {
        std::cerr << "Failed to initialize I2C." << std::endl;
        return -1;
    }
    return fd;
}

int readDistance(int fd) {
    int distance = wiringPiI2CRead(fd);
    return (distance > 0) ? distance : 1000;  // 1000 mm if read fail
}

void sendCommand(const std::string &command) {
    serialPort << command << std::endl;
    std::cout << "Sent command: " << command << std::endl;
}

void turnLeft(int speed = 2) {
    sendCommand("TurnZ_" + std::to_string(speed) + "_anti-horaire");
}

void turnRight(int speed = 2) {
    sendCommand("TurnZ_" + std::to_string(speed) + "_horaire");
}

void moveForward(int speed = 2, int repeat = 1) {
    sendCommand("Walk_" + std::to_string(speed) + "_" + std::to_string(repeat));
}

void stop() {
    sendCommand("Sleep_0");
}

void scanAndUpdateMap(int fd) {
    int distance = readDistance(fd);
    if (distance < 1000) {
        if (orientation == 0 && posY > 0) gridMap[posY-1][posX] = 1; // North obstacle
        else if (orientation == 1 && posX < MAP_WIDTH - 1) gridMap[posY][posX+1] = 1; // East obstacle
        else if (orientation == 2 && posY < MAP_HEIGHT - 1) gridMap[posY+1][posX] = 1; // South obstacle
        else if (orientation == 3 && posX > 0) gridMap[posY][posX-1] = 1; // West obstacle
    } else {
        if (orientation == 0 && posY > 0) gridMap[posY-1][posX] = 0;
        else if (orientation == 1 && posX < MAP_WIDTH - 1) gridMap[posY][posX+1] = 0;
        else if (orientation == 2 && posY < MAP_HEIGHT - 1) gridMap[posY+1][posX] = 0;
        else if (orientation == 3 && posX > 0) gridMap[posY][posX-1] = 0;
    }
}

void navigate(int fd) {
    scanAndUpdateMap(fd);
    if ((orientation == 0 && (posY == 0 || gridMap[posY-1][posX] == 1)) ||
        (orientation == 1 && (posX == MAP_WIDTH - 1 || gridMap[posY][posX+1] == 1)) ||
        (orientation == 2 && (posY == MAP_HEIGHT - 1 || gridMap[posY+1][posX] == 1)) ||
        (orientation == 3 && (posX == 0 || gridMap[posY][posX-1] == 1))) {
        turnRight();
    } else {
        moveForward();
    }
    usleep(500000);
}

int main() {
    serialPort.Open("/dev/ttyUSB0");
    serialPort.SetBaudRate(SerialStreamBuf::BAUD_9600);
    serialPort.SetCharSize(SerialStreamBuf::CHAR_SIZE_8);

    int fd = initVL53L0X();
    if (fd == -1) return 1;

    while (true) {
        navigate(fd);
        
        std::cout << "Map:" << std::endl;
        for (int i = 0; i < MAP_HEIGHT; ++i) {
            for (int j = 0; j < MAP_WIDTH; ++j) {
                std::cout << gridMap[i][j] << " ";
            }
            std::cout << std::endl;
        }
        usleep(1000000);
    }

    serialPort.Close();
    return 0;
}

