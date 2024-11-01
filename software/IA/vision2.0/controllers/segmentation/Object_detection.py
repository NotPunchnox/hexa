from ultralytics import YOLO
import cv2
from sklearn.cluster import KMeans
import numpy as np

model = YOLO("./model/yolov5nu_float16.tflite")
#model = YOLO("model/yolov8n.pt")


color_names = {
    "red": (255, 0, 0),
    "green": (0, 255, 0),
    "blue": (0, 0, 255),
    "yellow": (255, 255, 0),
    "cyan": (0, 255, 255),
    "magenta": (255, 0, 255),
    "black": (0, 0, 0),
    "white": (255, 255, 255),
    "gray": (128, 128, 128),
    "orange": (255, 165, 0),
    "purple": (128, 0, 128),
    "pink": (255, 192, 203),
    "brown": (165, 42, 42),
    "lime": (0, 255, 0),
    "navy": (0, 0, 128),
}

def estimate_size(bbox, image_shape):
    height, width = image_shape[:2]
    
    if height == 0 or width == 0:
        return "inconnu"

    bbox_area = (bbox[2] - bbox[0]) * (bbox[3] - bbox[1])
    image_area = height * width

    if image_area == 0:
        return "inconnu"

    size_ratio = bbox_area / image_area

    if size_ratio < 0.1:
        return "petit"
    elif size_ratio < 0.3:
        return "moyen"
    else:
        return "grand"

def Object_detection(cv2, frame):
    results = model(frame, show=False)
    
    for result in results:
        boxes = result.boxes

        for i, box in enumerate(boxes):
            x1, y1, x2, y2 = map(int, box.xyxy[0].cpu().numpy())  # Convertir en NumPy array
            conf = box.conf[0].item()  # Convertir tensor en float
            cls = int(box.cls[0].item())  # Convertir tensor en int
            label = model.names[cls]
            segmented_image = frame[y1:y2, x1:x2]

            if conf > 0.50:
                #size = estimate_size(box.xyxy[0].cpu().numpy(), frame.shape)
                cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
                cv2.putText(frame, f"{label} {conf:.2f}", (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
    
    return frame
