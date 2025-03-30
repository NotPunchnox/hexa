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
    raise Exception("Veuillez brancher le microcontroleur du robot : Connexion au CP210x manquante !")

# Gestionnaire WebSocket pour chaque client
async def websocket_handler(websocket, path):
    robot = None

    # Initialiser la connexion série si non en simulation
    if not args.simulation:
        try:
            serial_port_path = await find_serial_port()
            robot = serial.Serial(serial_port_path, baudrate=115200, timeout=1)
            print(f"Connecté à {serial_port_path}")
        except Exception as e:
            print(f"Erreur de connexion série : {e}")
            await websocket.close()
            return
    else:
        print("Mode simulation activé : aucune connexion série établie.")

    # Lancer une tâche pour lire le port série
    serial_task = None
    if not args.simulation and robot:
        serial_task = asyncio.create_task(read_from_serial(robot, websocket))

    try:
        while True:
            # Recevoir et traiter les messages du WebSocket
            print("En attente d'un message WebSocket...")
            message = await websocket.recv()
            print(f"Nouveau message reçu : {message}")

            if message == "ping":
                print("Réponse au ping envoyée.")
                await websocket.send("pong")
            else:
                try:
                    data = json.loads(message)
                    print(f"Message JSON décodé : {data}")

                    if 'animations' in data and isinstance(data['animations'], list):
                        # Envoyer directement les animations au robot
                        for animation in data['animations']:
                            print(f"Envoi de l'animation : {animation}")

                            if robot and not args.simulation:
                                try:
                                    robot.write((animation + '\n').encode())  # Ajouter un délimiteur '\n'
                                except Exception as e:
                                    print(f"Erreur d'écriture sur le port série : {e}")
                except json.JSONDecodeError as e:
                    print(f"Erreur de décodage JSON : {e}")
    except websockets.exceptions.ConnectionClosed:
        print("Connexion WebSocket fermée.")
    finally:
        # Nettoyer les tâches et fermer le port série
        print("Nettoyage des ressources...")
        if serial_task:
            serial_task.cancel()
            try:
                await serial_task
            except asyncio.CancelledError:
                pass
        if robot and not args.simulation:
            robot.close()
        print("Connexion client terminée.")

# Fonction asynchrone pour lire les données du port série
async def read_from_serial(robot, websocket):
    try:
        while True:
            if robot.in_waiting > 0:
                line = robot.readline().decode('utf-8').strip()
                print(f"Message reçu du port série : {line}")

                # Envoyer le message reçu au client WebSocket
                if websocket.open:
                    await websocket.send(line)
            await asyncio.sleep(0.01)  # Pause légère pour éviter une boucle rapide inutile
    except Exception as e:
        print(f"Erreur lors de la lecture du port série : {e}")

# Fonction principale
async def main():
    try:
        print("Démarrage du serveur WebSocket...")
        async with websockets.serve(websocket_handler, "0.0.0.0", 8080):
            print("Serveur WebSocket en cours d'exécution sur ws://localhost:8080")
            await asyncio.Future()  # Exécute indéfiniment
    except Exception as e:
        print(f"Erreur fatale : {e}")

if __name__ == "__main__":
    asyncio.run(main())
