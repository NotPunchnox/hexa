import asyncio
import websockets
import serial
import serial.tools.list_ports
import json
import argparse
from asyncio import Queue

# Analyser les arguments de la ligne de commande
parser = argparse.ArgumentParser(description="WebSocket server avec option de simulation.")
parser.add_argument('--simulation', action='store_true', help="Activer le mode simulation (désactive la communication série)")
args = parser.parse_args()

# File d'attente globale pour les animations
animations_queue = Queue()
animation_en_cours = False  # Indicateur d'une animation en cours

# Fonction pour trouver le port série
async def find_serial_port():
    ports = serial.tools.list_ports.comports()
    for port in ports:
        if 'CP210' in port.description:
            return port.device
    raise Exception("Veuillez brancher le microcontrôleur du robot : Connexion au CP210x manquante !")

# Fonction pour gérer les messages WebSocket
async def websocket_handler(websocket, path):
    global animation_en_cours
    robot = None

    # Initialiser la connexion série si non en simulation
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

    # Lancer une tâche pour lire les messages du port série
    serial_task = None
    if not args.simulation and robot:
        serial_task = asyncio.create_task(read_from_serial(robot, websocket))

    # Lancer une tâche pour gérer les animations
    animation_task = asyncio.create_task(process_animation_queue(robot, websocket))

    try:
        while True:
            # Attendre les messages WebSocket
            message = await websocket.recv()
            print("Nouveau message :", message)
            if message == "ping":
                await websocket.send("pong")
            else:
                try:
                    # Traiter le message JSON
                    data = json.loads(message)
                    print("Données parsées :", data)

                    if 'animations' in data and isinstance(data['animations'], list):
                        # Ajouter les animations à la file d'attente
                        for animation in data['animations']:
                            await animations_queue.put(animation)
                        print(f"{len(data['animations'])} animations ajoutées à la file d'attente.")
                except json.JSONDecodeError as err:
                    print(f"Erreur de décodage JSON : {err}")

    except websockets.exceptions.ConnectionClosed:
        print("Connexion WebSocket fermée.")
    finally:
        # Terminer les tâches et fermer les ressources
        if serial_task:
            serial_task.cancel()
        if animation_task:
            animation_task.cancel()
        if robot and not args.simulation:
            robot.close()

# Fonction asynchrone pour lire les données du port série
async def read_from_serial(robot, websocket):
    global animation_en_cours

    while True:
        if robot.in_waiting > 0:
            try:
                line = robot.readline().decode('utf-8').strip()
                print(f"Message reçu du port série : {line}")

                # Traiter la fin d'une animation
                if 'terminé' in line:
                    animation_en_cours = False  # Marquer l'animation comme terminée

                # Envoyer le message reçu au client WebSocket
                if websocket.open:
                    await websocket.send(line)
            except Exception as e:
                print(f"Erreur lors de la lecture du port série : {e}")
        await asyncio.sleep(0.01)  # Pause pour éviter une boucle rapide inutile

# Fonction pour gérer la file d'attente des animations
async def process_animation_queue(robot, websocket):
    global animation_en_cours

    while True:
        if not animation_en_cours and not animations_queue.empty():
            animation = await animations_queue.get()  # Récupérer la prochaine animation
            print(f"Envoi de l'animation : {animation}")

            if robot and not args.simulation:
                try:
                    robot.write((animation + '\n').encode())  # Ajouter un délimiteur '\n'
                except Exception as e:
                    print(f"Erreur d'écriture sur le port série : {e}")

            animation_en_cours = True  # Marquer une animation comme en cours

        await asyncio.sleep(0.01)  # Pause légère pour éviter une surcharge

# Fonction principale
async def main():
    async with websockets.serve(websocket_handler, "0.0.0.0", 8080):
        print("Serveur WebSocket en cours d'exécution sur ws://localhost:8080")
        await asyncio.Future()  # Exécute indéfiniment

if __name__ == "__main__":
    asyncio.run(main())
