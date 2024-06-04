import serial
import time

# Configurez le port série
ser = serial.Serial('COM11', 115200)

def send_command(command):
    ser.write((command + '\n').encode())
    time.sleep(1)

    response = ser.read_all().decode(errors='ignore').strip()
    return response

if __name__ == "__main__":
    while True:
        #command = input("Entrez une commande pour l'ESP8266: ")
        actions = ["ChangeTop_2_3", "ChangeTop_2.5_-3"]
        for command in actions:
            response = send_command(command)

        print("Hardware:", response)
