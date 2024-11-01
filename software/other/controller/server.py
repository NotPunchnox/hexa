import asyncio
import websockets
import serial
import serial.tools.list_ports
import json
import argparse

# Analyser les arguments de la ligne de commande
parser = argparse.ArgumentParser(description="WebSocket server avec option de simulation.")
parser.add_argument('--simulation', action='store_true', help="Activer le mode simulation (désactive la communication série)")
args = parser.parse_args()

# Fonction pour trouver le port série
async def find_serial_port():
    ports = serial.tools.list_ports.comports()
    for port in ports:
        if 'CP210' in port.description:
            return port.device
    raise Exception("Veuillez brancher le microcontrôleur du robot : Connexion au CP210x manquante !")

# Fonction pour gérer les messages WebSocket
async def echo(websocket, path):
    # Initialiser la connexion série si non en simulation
    robot = None
    if not args.simulation:
        try:
            serial_port_path = await find_serial_port()
            robot = serial.Serial(serial_port_path, baudrate=115200, timeout=1)
            print(f"Connecté à {serial_port_path}")
        except Exception as e:
            print(str(e))
            await websocket.close()
            return
    else:
        print("Mode simulation activé : aucune connexion série établie.")

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
                    await execute_animations(robot, data['animations'])
            except json.JSONDecodeError as err:
                print(f"Erreur de décodage JSON: {err}")

    except websockets.exceptions.ConnectionClosed:
        print("Connexion fermée.")
    finally:
        if robot and not args.simulation:
            robot.close()

async def execute_animations(robot, animations):
    for animation in animations:
        await send_animation(robot, animation)  # Envoie chaque animation une par une

async def send_animation(robot, animation):
    print(f"Envoi de l'animation: {animation}")
    if robot and not args.simulation:
        robot.write(animation.encode())
    await asyncio.sleep(0.8)

async def main():
    async with websockets.serve(echo, "0.0.0.0", 8080):
        print("Serveur WebSocket en cours d'exécution sur ws://localhost:8080")
        await asyncio.Future()  # Exécute indéfiniment

if __name__ == "__main__":
    asyncio.run(main())