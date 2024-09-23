import yolov5
import cv2
import numpy as np

model = yolov5.load('../model/yolov5n.pt')

# Paramètres du modèle
model.conf = 0.25  # niveau de "confiance" NMS
model.iou = 0.45  # niveau IoU NMS
model.agnostic = False  # NMS indépendant de la classe
model.multi_label = False  # une seule étiquette par box
model.max_det = 10  # Max de détections par images

last_detection = []

classes = []
with open('../model/coco.names', 'r') as f:
    classes = [line.strip() for line in f.readlines()]


def detect_and_display(frame):
    results = model(frame)
    predictions = results.pred[0]
    boxes = predictions[:, :4]  # x1, y1, x2, y2
    scores = predictions[:, 4]
    categories = predictions[:, 5]

    objects_detected = []    

    for box, score, category in zip(boxes, scores, categories):
        x1, y1, x2, y2 = box.int().tolist()
        label = f"{classes[int(category)]} {score:.2f}"
        color = (0, 255, 0)
        cv2.rectangle(frame, (x1, y1), (x2, y2), color, 2)
        cv2.putText(frame, label, (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, color, 2)

        name = classes[int(category)]
        objects_detected.append(name)
        
    if last_detection != objects_detected:
        print(f"Detected: {objects_detected}")
        #print(f"Detected: {name} with confidence {score:.2f} at [{x1}, {y1}, {x2}, {y2}]")

    rendered_frame = np.squeeze(results.render())
    return rendered_frame

def read_video_stream():
    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
        print("Erreur: Impossible d'ouvrir le flux vidéo")
        return

    while True:
        ret, frame = cap.read()
        if not ret:
            print("Erreur: Échec de la capture d'image")
            break

        frame = detect_and_display(frame)
        cv2.imshow("Object Detection", frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    read_video_stream()
