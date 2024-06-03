# Speaker - Client PC pour l'API du Robot Hexa

## Description

Speaker est un client PC conçu pour interagir avec l'API du robot Hexa. Ce client permet d'envoyer des commandes et de recevoir des réponses en utilisant la reconnaissance vocale. Après l'ajustement pour le bruit ambiant, vous pouvez parler dans le micro et obtenir une réponse du robot.
Le robot est connecté à l'intelligence artificielle Gemini Flash-1.5, vous pouvez créer vôtre propre modèle et l'entrainer pour rendre `Hexa` de plus en plus performant chaque jours.

## Installation

Pour installer les dépendances nécessaires, exécutez la commande suivante :

```bash
pip install -r requirements.txt
```

## Utilisation

Pour lancer l'application, utilisez la commande suivante :

```bash
py app.py
```

L'application ajustera automatiquement pour le bruit ambiant. Une fois l'ajustement terminé, parlez dans le micro pour obtenir une réponse du robot.

## Authentification Google Cloud

Pour authentifier l'application avec Google Cloud, utilisez la commande suivante :

```bash
gcloud auth application-default login
```

Cette commande ouvrira une fenêtre de navigateur pour vous permettre de vous connecter à votre compte Google Cloud et de donner les permissions nécessaires.

## Structure du Projet

- `app.py` : Script principal pour lancer l'application.
- `requirements.txt` : Fichier contenant toutes les dépendances nécessaires pour exécuter l'application.

## Fonctionnalités

- Ajustement automatique pour le bruit ambiant.
- Reconnaissance vocale pour envoyer des commandes et recevoir des réponses du robot Hexa.
- Authentification via Google Cloud pour accéder aux services nécessaires.

## Prérequis

- Python 3.6 ou supérieur
- Compte Google Cloud avec les permissions appropriées

## Dépendances

Les dépendances nécessaires sont listées dans le fichier `requirements.txt`. Assurez-vous de les installer en utilisant la commande `pip install -r requirements.txt`.