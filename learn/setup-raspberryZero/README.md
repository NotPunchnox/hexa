# Explication du Script de Configuration pour Raspberry Pi Zero

Ce document explique le processus de configuration automatisé pour un Raspberry Pi Zero sous Raspbian OS, en utilisant le script `setup.sh` et les fichiers associés. L'objectif est de configurer un système permettant de diffuser une caméra (via `camera.py`) et de gérer des commandes via un serveur WebSocket (via `server.py`).

## Étapes du Script de Configuration

### 1. Vérification et Installation de Python
Le script vérifie si Python 3 est installé sur le système. S'il n'est pas présent, il met à jour les dépôts et installe Python 3 ainsi que pip, l'outil de gestion des packages Python.

### 2. Mise à Jour de pip
Le script met à jour pip à la dernière version pour garantir la compatibilité avec les packages à installer.

### 3. Installation des Dépendances Python
Les packages Python suivants sont installés (si ce n'est pas déjà fait) :
- `picamera` : Pour interagir avec la caméra du Raspberry Pi.
- `io` : Pour la gestion des entrées/sorties.
- `logging` : Pour la journalisation des événements.
- `socketserver` : Pour créer des serveurs réseau.
- `server` : Pour des fonctionnalités de serveur génériques.
- `serial` : Pour la communication avec des périphériques série.
- `websockets` : Pour gérer les connexions WebSocket.
- `asyncio` : Pour la programmation asynchrone.
- `argparse` : Pour parser les arguments en ligne de commande.

Ces packages sont installés localement pour l'utilisateur via `pip3 install --user`.

### 4. Création du Répertoire et Téléchargement des Fichiers
- Un répertoire `~/Programs` est créé s'il n'existe pas.
- Les fichiers `camera.py` et `server.py` sont téléchargés depuis les URLs suivantes :
  - [camera.py](https://raw.githubusercontent.com/NotPunchnox/hexa/refs/heads/main/software/Raspberry-pi-zero/camera.py)
  - [server.py](https://raw.githubusercontent.com/NotPunchnox/hexa/refs/heads/main/software/Raspberry-pi-zero/server.py)
- Ces fichiers sont placés dans `~/Programs`.

### 5. Configuration des Services Systemd
Deux services systemd sont créés pour exécuter automatiquement les scripts Python au démarrage :
- **aash_vision.service** : Exécute `camera.py` pour diffuser le flux de la caméra.
- **aash_server.service** : Exécute `server.py` pour gérer les connexions WebSocket.

Les fichiers de service sont configurés comme suit :
- **Répertoire de travail** : `/home/admin/Programs`
- **Utilisateur** : `admin` (peut être modifié en `pi` si nécessaire)
- **Chemin d'environnement** : Inclut les chemins pour les binaires Python locaux et système.
- **Redémarrage** : Les services redémarrent automatiquement en cas d'échec.

Les services sont activés (`enable`) et démarrés (`start`), et leur statut est affiché (`status`) pour vérification.

### 6. Résultat Final
Une fois le script exécuté, le Raspberry Pi Zero est configuré pour :
- Exécuter un service de diffusion de caméra via `aash_vision.service`.
- Exécuter un serveur WebSocket via `aash_server.service`.
- Les scripts s'exécutent automatiquement au démarrage du système.

## Utilisation du Script de Configuration
Pour exécuter le script `setup.sh` (disponible à [https://raw.githubusercontent.com/NotPunchnox/hexa/refs/heads/main/software/Raspberry-pi-zero/setup.sh](https://raw.githubusercontent.com/NotPunchnox/hexa/refs/heads/main/software/Raspberry-pi-zero/setup.sh)) :
1. Téléchargez le script : `wget https://raw.githubusercontent.com/NotPunchnox/hexa/refs/heads/main/software/Raspberry-pi-zero/setup.sh`
2. Rendez-le exécutable : `chmod +x setup.sh`
3. Exécutez-le avec les privilèges root : `sudo ./setup.sh`

## Remarques Importantes
- **Utilisateur** : Le script suppose que l'utilisateur est `admin`. Si vous utilisez un autre utilisateur (par exemple, `pi`), modifiez les fichiers de service (`User=`) ou le script avant exécution.
- **Connexion réseau** : Les services nécessitent une connexion réseau active, car ils dépendent de `network.target`.
- **Vérification** : Vérifiez les statuts des services avec `sudo systemctl status aash_vision.service` et `sudo systemctl status aash_server.service` pour vous assurer qu'ils fonctionnent correctement.
- **Dépannage** : Si les services ne démarrent pas, vérifiez les journaux avec `journalctl -u aash_vision.service` ou `journalctl -u aash_server.service`.

Ce script automatise la configuration d'un système de caméra et de serveur WebSocket sur un Raspberry Pi Zero, facilitant le déploiement d'applications IoT ou de surveillance.
