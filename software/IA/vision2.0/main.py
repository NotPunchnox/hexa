import cv2
import time
import json
from flask import Flask, jsonify
from controllers.segmentation.Object_detection import Object_detection
from controllers.Face.face_detector import FaceRecognition
from controllers.Vars import variables

# Initialize Flask app
app = Flask(__name__)

# Initialize the FaceRecognition and other components
Face = FaceRecognition()

# Global variables to store results
detected_faces = []
detected_objects = []

# Open the camera
webcam = cv2.VideoCapture(0)

width = int(webcam.get(cv2.CAP_PROP_FRAME_WIDTH))
height = int(webcam.get(cv2.CAP_PROP_FRAME_HEIGHT))
print(f"Camera dimensions: width = {width}, height = {height}")

# Function to start the webcam and process frames
def process_webcam():
    global detected_faces, detected_objects

    while webcam.isOpened():
        ret, frame = webcam.read()
        if not ret:
            break
        start_time = time.time()
        
        # Resize the frame
        frame2 = cv2.resize(frame, (320, 240))
        
        # Object detection and face recognition
        frame = Object_detection(cv2, frame)
        frame = Face.face_recognization(frame)

        # Update detected faces and objects
        detected_faces = variables.names
        detected_objects = variables.objects
        
        # Display FPS
        end_time = time.time()
        fps = 1 / float(end_time - start_time)
        cv2.putText(frame, f"FPS: {fps:.2f}", (240, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 255), 2)

        cv2.imshow("Aash Vision; by punchnox", frame)
        
        # Exit the program with 'q' key
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    webcam.release()
    cv2.destroyAllWindows()

# API endpoint to get detected faces and objects
@app.route('/api/results', methods=['GET'])
def get_results():
    return jsonify({
        'faces': detected_faces,
        'objects': detected_objects
    })

if __name__ == '__main__':
    import threading

    threading.Thread(target=process_webcam, daemon=True).start()

    app.run(host='0.0.0.0', port=3006)
