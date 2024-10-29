import mediapipe as mp
import face_recognition as fr
import os
import time
import cv2

class FaceRecognition:
    def __init__(self, analysis_interval=3):
        self.mp_face_detection = mp.solutions.face_detection
        self.mp_drawing = mp.solutions.drawing_utils
        self.analysis_interval = analysis_interval
        self.last_analysis_time = time.time()
        self.known_face_encodings = []
        self.known_face_names = []

        # Charger les visages connus
        path = os.path.join(os.getcwd(), "./db/faces")
        for file_name in os.listdir(path):
            image_path = os.path.join(path, file_name)
            image = fr.load_image_file(image_path)
            try:
                face_encoding = fr.face_encodings(image)[0]
                self.known_face_encodings.append(face_encoding)
                self.known_face_names.append(os.path.splitext(file_name)[0])
            except IndexError:
                print(f"Aucun visage trouvé dans {file_name}.")

    def face_recognization(self, frame):
        with self.mp_face_detection.FaceDetection(min_detection_confidence=0.5) as face_detection:
            frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
            results = face_detection.process(frame_rgb)

            face_locations = []

            if results.detections:
                for detection in results.detections:
                    self.mp_drawing.draw_detection(frame, detection)

                    bboxC = detection.location_data.relative_bounding_box
                    h, w, _ = frame.shape
                    top = int(bboxC.ymin * h)
                    left = int(bboxC.xmin * w)
                    bottom = int((bboxC.ymin + bboxC.height) * h)
                    right = int((bboxC.xmin + bboxC.width) * w)

                    face_locations.append((top, right, bottom, left))

            # Effectuer la reconnaissance toutes les `analysis_interval` secondes
            current_time = time.time()
            if current_time - self.last_analysis_time >= self.analysis_interval and face_locations:
                face_encodings = fr.face_encodings(frame_rgb, face_locations)

                for (top, right, bottom, left), face_encoding in zip(face_locations, face_encodings):
                    matches = fr.compare_faces(self.known_face_encodings, face_encoding)
                    name = "Inconnu"
                    face_distances = fr.face_distance(self.known_face_encodings, face_encoding)
                    best_match_index = face_distances.argmin()
                    if matches[best_match_index]:
                        name = self.known_face_names[best_match_index]
                    
                    print("Personne détectée :", name)
                    # Dessiner le rectangle et le nom du visage reconnu
                    # cv2.rectangle(frame, (left, top), (right, bottom), (0, 255, 0), 2)
                    # cv2.putText(frame, name, (left, top - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.9, (0, 255, 0), 2)
                
                # Mettre à jour le temps de la dernière analyse
                self.last_analysis_time = current_time

        return frame
