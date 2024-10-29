from ultralytics import YOLO
import cv2

# Load the YOLO11 model
#model = YOLO("./model/yolov5nu.pt")

# Export the model to TFLite format
#model.export(format="tflite")  # creates 'yolo11n_float32.tflite'

# Load the exported TFLite model
tflite_model = YOLO("./model/yolov5nu_saved_model/yolov5nu_float32.tflite")

webcam = cv2.VideoCapture(0)

while True:
    ret, frame = webcam.read()
    if not ret:
        break 

    res = tflite_model(frame)
    print(res)
    

    cv2.imshow("Aash Vision: test", frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break 

webcam.release()
cv2.destroyAllWindows()
