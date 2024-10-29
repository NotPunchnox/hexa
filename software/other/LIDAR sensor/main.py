import asyncio
import websockets
import serial
import serial.tools.list_ports
import json

# Fonction pour trouver le port série
async def find_serial_port():
    ports = serial.tools.list_ports.comports()
    for port in ports:
        if 'CP210' in port.description:
            return port.device
    raise Exception("Veuillez brancher le microcontrôleur du robot : Connexion au CP210x manquante !")

# Fonction pour gérer les messages WebSocket
async def echo(websocket, path):
    # Initialiser la connexion série
    try:
        serial_port_path = await find_serial_port()
        robot = serial.Serial(serial_port_path, baudrate=115200, timeout=1)
        print(f"Connecté à {serial_port_path}")
    except Exception as e:
        print(str(e))
        await websocket.close()
        return

    try:
        while True:
            # Attendre les messages du client WebSocket
            message = await websocket.recv()
            print("Nouveau message:", message)

            # Traiter le message JSON
            try:
                data = json.loads(message)
                print("Données parse:", data)
                if 'animations' in data and isinstance(data['animations'], list):
                    for animation in data['animations']:
                        robot.write(animation.encode())
                        await asyncio.sleep(0.2)  # Attendre 200ms avant d'envoyer la prochaine animation
            except json.JSONDecodeError as err:
                print(f"Erreur de décodage JSON: {err}")

            # Lire les données du robot
#            while robot.in_waiting > 0:
#                try:
#                    data_robot = robot.readline().decode('utf-8').strip()  # Essayer de décoder en UTF-8
#                    print('Nouvelles données venant du robot:', data_robot)
#                    await websocket.send(json.dumps({"type": "LOGS", "data": data_robot}))
#                except UnicodeDecodeError as e:
#                    print(f"Erreur de décodage des données du robot: {e}")
#                    # Ici vous pouvez décider de gérer la situation (ignorer, enregistrer l'erreur, etc.)
#                    continue  # Ignorer cette itération et passer à la suivante

    except websockets.exceptions.ConnectionClosed:
        print("Connexion fermée.")
    finally:
        robot.close()

async def main():
    async with websockets.serve(echo, "0.0.0.0", 8080):
        print("Serveur WebSocket en cours d'exécution sur ws://localhost:8080")
        await asyncio.Future()  # Exécute indéfiniment

if __name__ == "__main__":
    asyncio.run(main())
