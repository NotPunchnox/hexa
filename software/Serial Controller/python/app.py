import time, os, serial

ser = serial.Serial('COM11', 115200)

#envoi de réponse sur le port com série
def send_command(command):
    
    ser.write((command + '\n').encode())
    time.sleep(1)

    response = ser.read_all().decode(errors='ignore').strip()
    print("Hardware:", response)
    return response

ActionsXY = [
    #"TurnZ_1_horaire",
    "ChangeXY_2_0",
    "ChangeXY_4_5",
    "ChangeXY_8_-5",
    "ChangeXY_8_5",
    "ChangeXY_8_-5",
    "ChangeXY_8_5",
    "ChangeTop_3_2",
    "ChangeTop_6_-4",
    "ChangeTop_3_2",
    "ChangeTop_3_-2",
    "ChangeXY_8_-5",
    "ChangeXY_8_5",
    "ChangeXY_8_-5",
    "ChangeTop_2_2",
    "Turn_left_2_3",

    # "ChangeXY_4_0_0",
    # "ChangeXY_4_4_4",
    # "ChangeXY_4_-4_-4",
    # "ChangeXY_4_4_0",
    # "ChangeXY_4_0_4",
    # "ChangeXY_4_-4_-4",

    # "ChangeTop_2_4",
    # "ChangeTop_3_-5",
    # "ChangeTop_2_2",

    # "Rouli_1_3_-3_0_0",
    # "Rouli_1_-3_3_0_0",
    # "Rouli_0.5_0_0_0_0",
    # "Rouli_1_0_0_3_-3",
    # "Rouli_1_0_0_-3_3",
    
    # "TurnZ_1_anti-horaire",
    # "ChangeTop_4_-3",
    "Sleep_3"
]

# Fonction main
def main():
    os.system('cls')
    time.sleep(3)

    # while True:

    #     text = input("Vous: ")
    #     send_command(text)

    for command in ActionsXY:
        send_command(command)
    
    time.sleep(10)

if __name__ == "__main__":
    main()