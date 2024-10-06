# Mise en place de la caméra pour la vision du robot Aash

## Matériel nécessaire :
- 1 Raspberry Pi Zero W/2W
- 1 carte micro SD (8, 16, 32, ou 64 Go)
- 1 câble d'alimentation micro-USB ou USB-C selon le modèle
- Une caméra compatible Raspberry Pi
- Un accès à Internet pour les mises à jour et les installations (Ethernet ou un dongle Wi-Fi si vous utilisez RaspAP)

## Étape 1 : Flasher la carte SD avec Raspberry Pi OS Lite (CLI)
Téléchargez et installez **Raspberry Pi Imager** pour flasher votre carte SD avec **Raspberry Pi OS Lite** (sans interface graphique).

**Instructions pour flasher la carte SD** :
- Sélectionnez Raspberry Pi OS Lite dans Raspberry Pi Imager.
- Choisissez la carte SD appropriée.
- Flashez la carte SD.

## Étape 2 : Connexion SSH au Raspberry Pi

1. Démarrez le Raspberry Pi avec la carte SD et connectez-vous en SSH. Si nécessaire, configurez les options réseau pour accéder à votre Pi en SSH :
   ```bash
   ssh pi@raspberrypi.local
   ```

2. **Activer la caméra** sur le Raspberry Pi :
   ```bash
   sudo raspi-config
   ```

   Dans le menu, allez dans :
   - **Interface Options** > **Enable Camera**
   
   Confirmez, puis redémarrez le Raspberry Pi :
   ```bash
   sudo reboot
   ```

## Étape 3 : Installer Python et les dépendances

1. **Mise à jour du Raspberry Pi** :
   Avant d’installer les paquets nécessaires, mettez à jour votre système :
   ```bash
   sudo apt update
   sudo apt upgrade
   ```

2. **Installer Python** :
   Si Python n'est pas installé, installez-le :
   ```bash
   sudo apt install python pip
   ```

3. **Installer les dépendances pour la caméra** :
   Utilisez la bibliothèque **PiCamera** pour interagir avec la caméra Raspberry Pi :
   ```bash
   pip install picamera io logging socketserver server serial
   ```

## Étape 4 : Créer un script pour le streaming de la caméra

1. Créez un dossier pour organiser vos programmes :
   ```bash
   mkdir ~/Programs
   cd ~/Programs
   ```

2. Créez un fichier Python pour gérer le streaming de la caméra :
   ```bash
   touch camera.py
   ```

3. Éditez le fichier `camera.py` et ajoutez le code suivant :

### Code pour le streaming de la caméra :

```python
import io
import picamera
import logging
import socketserver
from threading import Condition
from http import server

# Page HTML pour afficher le flux vidéo
PAGE="""\
<html>
<head>
<title>Raspberry Pi - Aash Vision</title>
</head>
<body>
<center><h1>Raspberry Pi - Aash Vision</h1></center>
<center><img src="stream.mjpg" width="640" height="480"></center>
</body>
</html>
"""

class StreamingOutput(object):
    def __init__(self):
        self.frame = None
        self.buffer = io.BytesIO()
        self.condition = Condition()

    def write(self, buf):
        if buf.startswith(b'\xff\xd8'):
            self.buffer.truncate()
            with self.condition:
                self.frame = self.buffer.getvalue()
                self.condition.notify_all()
            self.buffer.seek(0)
        return self.buffer.write(buf)

class StreamingHandler(server.BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path == '/':
            self.send_response(301)
            self.send_header('Location', '/index.html')
            self.end_headers()
        elif self.path == '/index.html':
            content = PAGE.encode('utf-8')
            self.send_response(200)
            self.send_header('Content-Type', 'text/html')
            self.send_header('Content-Length', len(content))
            self.end_headers()
            self.wfile.write(content)
        elif self.path == '/stream.mjpg':
            self.send_response(200)
            self.send_header('Age', 0)
            self.send_header('Cache-Control', 'no-cache, private')
            self.send_header('Pragma', 'no-cache')
            self.send_header('Content-Type', 'multipart/x-mixed-replace; boundary=FRAME')
            self.end_headers()
            try:
                while True:
                    with output.condition:
                        output.condition.wait()
                        frame = output.frame
                    self.wfile.write(b'--FRAME\r\n')
                    self.send_header('Content-Type', 'image/jpeg')
                    self.send_header('Content-Length', len(frame))
                    self.end_headers()
                    self.wfile.write(frame)
                    self.wfile.write(b'\r\n')
            except Exception as e:
                logging.warning(
                    'Removed streaming client %s: %s',
                    self.client_address, str(e))
        else:
            self.send_error(404)
            self.end_headers()

class StreamingServer(socketserver.ThreadingMixIn, server.HTTPServer):
    allow_reuse_address = True
    daemon_threads = True

# Démarrer la caméra et le serveur HTTP
with picamera.PiCamera(resolution='640x480', framerate=24) as camera:
    output = StreamingOutput()
    camera.start_recording(output, format='mjpeg')
    try:
        address = ('', 8000)
        server = StreamingServer(address, StreamingHandler)
        server.serve_forever()
    finally:
        camera.stop_recording()
```

### Explication :
- Ce code met en place un serveur HTTP sur le Raspberry Pi qui diffuse un flux vidéo MJPEG de la caméra.
- Vous pouvez accéder au flux en ouvrant un navigateur à l'adresse : `http://<adresse-ip-du-raspberry>:8000`.

## Étape 5 : Tester le flux vidéo

1. Exécutez le script Python :
   ```bash
   python camera.py
   ```

2. Accédez au flux vidéo via votre navigateur web :
   ```
   http://<IP_du_RaspberryPi>:8000
   ```
   Remplacez `<IP_du_RaspberryPi>` par l'adresse IP réelle de votre Raspberry Pi.

## Étape 6 : Lancer automatiquement le script au démarrage

Pour lancer le script de streaming au démarrage du Raspberry Pi, utilisez **systemd**.

1. Créez un fichier service pour votre script :
   ```bash
   sudo nano /etc/systemd/system/aash_vision.service
   ```

2. Ajoutez le contenu suivant au fichier ( **Pensez à modifier ADMIN or PI en fonction de vôtre nom d'utilisateur**) :

```bash
[Unit]
Description=Aash Vision Camera Stream
After=network.target

[Service]
ExecStart=/usr/bin/python3 /home/admin/Programs/camera.py
WorkingDirectory=/home/admin/Programs
User=admin or pi
Environment="PATH=/home/admin/.local/bin:/usr/bin:/bin"
Restart=on-failure

[Install]
WantedBy=multi-user.target
```

3. Enregistrez et fermez le fichier. Puis, activez le service :
   ```bash
   sudo systemctl enable aash_vision.service
   ```

4. Démarrez le service immédiatement pour tester :
   ```bash
   sudo systemctl start aash_vision.service
   ```

5. Vérifiez l'état du service :
   ```bash
   sudo systemctl status aash_vision.service
   ```

Le flux sera maintenant démarré automatiquement à chaque démarrage du Raspberry Pi.

---

### Conclusion

Vous avez maintenant un flux vidéo en direct à partir de la caméra de votre Raspberry Pi, prêt à être utilisé sur votre robot hexapode **Aash**. Vous pouvez accéder à ce flux via un navigateur web, et le script se lancera automatiquement au démarrage du système.


#By punchnox