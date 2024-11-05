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

animations = []
index_animation = 0
animation_en_cours = False  # Indicateur d'animation en cours

# Fonction pour trouver le port série
async def find_serial_port():
    ports = serial.tools.list_ports.comports()
    for port in ports:
        if 'CP210' in port.description:
            return port.device
    raise Exception("Veuillez brancher le microcontrôleur du robot : Connexion au CP210x manquante !")

# Fonction pour gérer les messages WebSocket
async def echo(websocket, path):
    global animations, index_animation, animation_en_cours
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

    # Lancer une tâche pour lire du port série
    serial_task = None
    if not args.simulation and robot:
        serial_task = asyncio.create_task(read_from_serial(robot, websocket))

    try:
        while True:
            # Attendre les messages du client WebSocket
            message = await websocket.recv()
            print("Nouveau message:", message)
            if message == "ping":
                await websocket.send("pong")
                
            else:
                # Traiter le message JSON
                try:
                    data = json.loads(message)
                    print("Données parse:", data)

                    if 'animations' in data and isinstance(data['animations'], list):
                        animations = data['animations']
                        index_animation = 0  # Réinitialise l'index
                        animation_en_cours = False  # Réinitialise l'état d'animation
                        await send_next_animation(robot, websocket)  # Démarrer la première animation
                except json.JSONDecodeError as err:
                    print(f"Erreur de décodage JSON: {err}")

    except websockets.exceptions.ConnectionClosed:
        print("Connexion WebSocket fermée.")
    finally:
        # Terminer la tâche de lecture série et fermer le port série
        if serial_task:
            serial_task.cancel()
        if robot and not args.simulation:
            robot.close()

# Fonction asynchrone pour lire les données du port série
async def read_from_serial(robot, websocket):
    global animations, index_animation, animation_en_cours

    while True:
        if robot.in_waiting > 0:
            try:
                line = robot.readline().decode().strip()
                print(f"Message reçu du port série : {line}")

                if 'terminé' in line and animation_en_cours:
                    index_animation += 1
                    animation_en_cours = False  # Marquer l'animation comme terminée
                    await send_next_animation(robot, websocket)  # Envoyer l'animation suivante

                # Envoyer le message reçu au client WebSocket
                if websocket.open:
                    await websocket.send(line)

            except Exception as e:
                print(f"Erreur lors de la lecture du port série : {e}")
        await asyncio.sleep(0.1)  # Pause pour éviter une boucle rapide inutile

# Fonction pour envoyer la prochaine animation
async def send_next_animation(robot, websocket):
    global animations, index_animation, animation_en_cours

    # Vérifier si nous avons encore des animations à envoyer
    if index_animation < len(animations):
        animation = animations[index_animation]
        print(f"Envoi de l'animation: {animation}")
        
        if robot and not args.simulation:
            robot.write(animation.encode())

        animation_en_cours = True  # Marquer une animation en cours
        # await asyncio.sleep(0.8)
    else:
        # Si toutes les animations sont terminées, envoyer une notification
        index_animation = 0
        if websocket.open:
            await websocket.send("Séquence d'animations terminée")

async def main():
    async with websockets.serve(echo, "0.0.0.0", 8080):
        print("Serveur WebSocket en cours d'exécution sur ws://localhost:8080")
        await asyncio.Future()  # Exécute indéfiniment

if __name__ == "__main__":
    asyncio.run(main())
