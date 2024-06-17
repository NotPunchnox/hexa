import cv2
import numpy as np
import time
import threading

# URL du flux vidéo
url = "http://192.168.1.52:8000/stream.mjpg"

# Charger le modèle de détection d'objets pré-entraîné
net = cv2.dnn.readNet("./model/yolov3.weights", "./model/yolov3.cfg")


net.setPreferableBackend(cv2.dnn.DNN_BACKEND_CUDA)
net.setPreferableTarget(cv2.dnn.DNN_TARGET_CUDA)

classes = []
with open("./model/coco.names", "r") as f:
    classes = [line.strip() for line in f.readlines()]

layer_names = net.getLayerNames()
unconnected_layers = net.getUnconnectedOutLayers()
output_layers = [layer_names[i - 1] for i in unconnected_layers]

frame = None
lock = threading.Lock()
streaming = True


def detect_objects(image):
    height, width, _ = image.shape
    blob = cv2.dnn.blobFromImage(image, 0.00392, (320, 320), (0, 0, 0), True, crop=False)
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
            cv2.putText(image, label, (x, y - 10), font, 1, color, 2)

    return image


def read_video_stream(url):
    global frame, lock, streaming
    cap = cv2.VideoCapture(url)
    if not cap.isOpened():
        print("Error: Could not open video stream")
        return

    while streaming:
        ret, new_frame = cap.read()
        if not ret:
            print("Failed to grab frame")
            break

        with lock:
            frame = new_frame

    cap.release()


def process_frames():
    global frame, lock, streaming
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

        if cv2.waitKey(25) & 0xFF == ord('q'):
            streaming = False
            break

    cv2.destroyAllWindows()


t1 = threading.Thread(target=read_video_stream, args=(url,))
t2 = threading.Thread(target=process_frames)

t1.start()
t2.start()

t1.join()
t2.join()
