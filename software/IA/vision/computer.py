import cv2
import time
import yolov5
import requests

model = yolov5.load('./model/yolov5n6.pt')

# Paramètres du modèle
model.conf = 0.25  # Seuil de confiance pour NMS
model.iou = 0.45  # Seuil IoU pour NMS
model.agnostic = False  # NMS indépendant de la classe
model.multi_label = False  # Une seule étiquette par boîte
model.max_det = 10  # Nombre maximum de détections par image

last_detection_names = set()

# Charger les noms des classes
with open('./model/coco.names', 'r') as f:
    classes = [line.strip() for line in f.readlines()]

# Délai minimum entre chaque traitement d'image par l'IA
process_delay = 0.5

last_process_time = time.time()

def detect_objects(frame):
    global last_process_time, last_detection_names
    current_time = time.time()
    if current_time - last_process_time < process_delay:
        return frame
    last_process_time = current_time

    results = model(frame)
    predictions = results.pred[0]
    boxes = predictions[:, :4]  # x1, y1, x2, y2
    scores = predictions[:, 4]
    categories = predictions[:, 5]

    objects_detected = []

    for box, score, category in zip(boxes, scores, categories):
        if score > 0.3:
            x1, y1, x2, y2 = box.int().tolist()
            # label = f"{classes[int(category)]} {score:.2f}"
            # color = (0, 255, 0)
            # cv2.rectangle(frame, (x1, y1), (x2, y2), color, 2)
            # cv2.putText(frame, label, (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, color, 2)

            name = classes[int(category)]
            object_detected = {
                "name": name,
                "position": {"x1": x1, "y1": y1, "x2": x2, "y2": y2},
                "confiance": float(score)
            }
            objects_detected.append(object_detected)

    current_detection_names = {obj["name"] for obj in objects_detected}

    if current_detection_names != last_detection_names:
        print(f"Detected: {objects_detected}")
        last_detection_names = current_detection_names

        url = "http://localhost:3000/api/vision"
        payload = {"array": objects_detected}
        headers = {
            "Content-Type": "application/json",
            "User-Agent": "insomnia/2023.5.8"
        }

        response = requests.get(url, json=payload, headers=headers)
        print(response.text)


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

        detect_objects(frame)
        cv2.imshow("Object Detection", frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    # URL du flux vidéo
    url = "http://192.168.1.52:8000/stream.mjpg"
    read_video_stream(url)
