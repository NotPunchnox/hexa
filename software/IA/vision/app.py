import cv2
import numpy as np
import time
import threading

# URL du flux vidéo
url = "http://192.168.1.52:8000/stream.mjpg"

# Charger le modèle de détection d'objets pré-entraîné
net = cv2.dnn.readNet("yolov3.weights", "yolov3.cfg")

# Utiliser CUDA si disponible
net.setPreferableBackend(cv2.dnn.DNN_BACKEND_CUDA)
net.setPreferableTarget(cv2.dnn.DNN_TARGET_CUDA)

classes = []
with open("coco.names", "r") as f:
    classes = [line.strip() for line in f.readlines()]

layer_names = net.getLayerNames()
unconnected_layers = net.getUnconnectedOutLayers()
output_layers = [layer_names[i - 1] for i in unconnected_layers]

frame = None
lock = threading.Lock()

# Fonction pour détecter les objets dans une image
def detect_objects(image):
    height, width, _ = image.shape
    resized_image = cv2.resize(image, (224, 224))  # Réduire la taille de l'image à 224x224
    blob = cv2.dnn.blobFromImage(resized_image, 0.00392, (224, 224), (0, 0, 0), True, crop=False)
    net.setInput(blob)
    outs = net.forward(output_layers)
    class_ids = []
    confidences = []
    boxes = []

    for out in outs:
        for detection in out:
            scores = detection[5:]
            class_id = np.argmax(scores)
            confidence = scores[class_id]

            if confidence > 0.5:
                center_x = int(detection[0] * width)
                center_y = int(detection[1] * height)
                w = int(detection[2] * width)
                h = int(detection[3] * height)
                x = int(center_x - w / 2)
                y = int(center_y - h / 2)
                boxes.append([x, y, w, h])
                confidences.append(float(confidence))
                class_ids.append(class_id)

    indexes = cv2.dnn.NMSBoxes(boxes, confidences, 0.5, 0.4)

    font = cv2.FONT_HERSHEY_PLAIN
    colors = np.random.uniform(0, 255, size=(len(classes), 3))

    for i in range(len(boxes)):
        if i in indexes:
            x, y, w, h = boxes[i]
            label = str(classes[class_ids[i]])
            color = colors[class_ids[i]]
            cv2.rectangle(image, (x, y), (x + w, y + h), color, 2)
            cv2.putText(image, label, (x, y - 10), font, 1, color, 2)  # Réduire la taille et l'épaisseur de la police

    return image

# Fonction pour lire le flux vidéo à partir de l'URL
def read_video_stream(url):
    global frame, lock
    cap = cv2.VideoCapture(url)
    if not cap.isOpened():
        print("Error: Could not open video stream")
        return

    while cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            print("Failed to grab frame")
            break

        with lock:
            frame_copy = frame.copy()
        time.sleep(0.01)

    cap.release()

# Fonction pour traiter et afficher les frames
def process_frames():
    global frame, lock
    while True:
        start_time = time.time()
        with lock:
            if frame is None:
                continue
            frame_copy = frame.copy()

        frame_copy = detect_objects(frame_copy)
        cv2.imshow("Object Detection", frame_copy)

        end_time = time.time()
        fps = 1 / (end_time - start_time)
        print(f"FPS: {fps:.2f}")

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cv2.destroyAllWindows()

# Démarrer les threads pour la capture vidéo et le traitement
t1 = threading.Thread(target=read_video_stream, args=(url,))
t2 = threading.Thread(target=process_frames)

t1.start()
t2.start()

t1.join()
t2.join()
