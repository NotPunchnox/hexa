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
    "ChangeXY_6_4_4",
    "ChangeXY_6_4_-4",
    "ChangeXY_6_-4_-4",
    "ChangeXY_6_-4_4"
]

# Fonction main
def main():
    os.system('cls')
    time.sleep(3)
    #text = input("Vous: ")
    send_command("ChangeXY_4_0_0")

    for command in ActionsXY:
        send_command(command)
    
    send_command("ChangeXY_4_0_0")    
    time.sleep(10)

if __name__ == "__main__":
    main()