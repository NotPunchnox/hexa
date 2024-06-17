import yolov5
import cv2
import numpy as np
import threading
import time

# Load YOLOv5 model
model = yolov5.load('./model/yolov5n.pt')

# Model parameters
model.conf = 0.25  # NMS confidence threshold
model.iou = 0.45  # NMS IoU threshold
model.agnostic = False  # Class-agnostic NMS
model.multi_label = False  # One label per box
model.max_det = 10  # Max detections per image

# Load class names
classes = []
with open('./model/coco.names', 'r') as f:
    classes = [line.strip() for line in f.readlines()]

frame = None
lock = threading.Lock()
streaming = True

def detect_and_display(image):
    results = model(image)
    predictions = results.pred[0]
    boxes = predictions[:, :4]  # x1, y1, x2, y2
    scores = predictions[:, 4]
    categories = predictions[:, 5]

    for box, score, category in zip(boxes, scores, categories):
        x1, y1, x2, y2 = box.int().tolist()
        label = f"{classes[int(category)]} {score:.2f}"
        color = (0, 255, 0)
        cv2.rectangle(image, (x1, y1), (x2, y2), color, 2)
        cv2.putText(image, label, (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, color, 2)

    rendered_frame = np.squeeze(results.render())
    return rendered_frame

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

        frame_copy = detect_and_display(frame_copy)
        cv2.imshow("Object Detection", frame_copy)

        end_time = time.time()
        fps = 1 / (end_time - start_time)
        print(f"FPS: {fps:.2f}")

        if cv2.waitKey(1) & 0xFF == ord('q'):
            streaming = False
            break

    cv2.destroyAllWindows()

if __name__ == "__main__":
    # URL of the video stream
    url = "http://192.168.1.52:8000/stream.mjpg"

    # Start threads for reading and processing video stream
    t1 = threading.Thread(target=read_video_stream, args=(url,))
    t2 = threading.Thread(target=process_frames)

    t1.start()
    t2.start()

    t1.join()
    t2.join()
