import cv2
import time
import yolov5
import numpy as np

# Charger le modèle YOLOv5n
model = yolov5.load('./model/yolov5n.pt')

# Paramètres du modèle
model.conf = 0.25  # Seuil de confiance pour NMS
model.iou = 0.45  # Seuil IoU pour NMS
model.agnostic = False  # NMS indépendant de la classe
model.multi_label = False  # Une seule étiquette par boîte
model.max_det = 10  # Nombre maximum de détections par image

# Charger les noms des classes
with open('./model/coco.names', 'r') as f:
    classes = [line.strip() for line in f.readlines()]

# Délai minimum entre chaque traitement d'image par l'IA
process_delay = 0.5  # 2 images par seconde

last_process_time = time.time()

def detect_objects(frame):
    global last_process_time

    # Vérifier si suffisamment de temps s'est écoulé depuis le dernier traitement
    current_time = time.time()
    if current_time - last_process_time < process_delay:
        return frame

    # Mettre à jour le dernier temps de traitement
    last_process_time = current_time

    # Détecter et afficher les objets
    results = model(frame)
    predictions = results.pred[0]

    for pred in predictions:
        x1, y1, x2, y2 = pred[:4].int().tolist()  # Récupérer et convertir les coordonnées en entiers
        score = float(pred[4])
        category = int(pred[5])

        label = f"{classes[category]} {score:.2f}"
        color = (0, 255, 0)
        cv2.rectangle(frame, (x1, y1), (x2, y2), color, 2)
        cv2.putText(frame, label, (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, color, 2)
        print(f"Detected: {classes[category]} with confidence {score:.2f} at [{x1}, {y1}, {x2}, {y2}]")

    return frame


def read_video_stream(url):
    cap = cv2.VideoCapture(url)
    if not cap.isOpened():
        print("Error: Could not open video stream")
        return

    while True:
        ret, frame = cap.read()
        if not ret:
            print("Failed to grab frame")
            break

        frame = detect_objects(frame)
        cv2.imshow("Video Stream", frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    # URL du flux vidéo
    url = "http://192.168.1.52:8000/stream.mjpg"
    read_video_stream(url)
