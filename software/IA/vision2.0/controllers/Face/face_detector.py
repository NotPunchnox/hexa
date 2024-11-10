import mediapipe as mp
import face_recognition as fr
import os
import time
import cv2
from controllers.Vars import variables

class FaceRecognition:
    def __init__(self, analysis_interval=3, known_face_width=0.15, focal_length=600):
        self.mp_face_detection = mp.solutions.face_detection
        self.mp_drawing = mp.solutions.drawing_utils
        self.analysis_interval = analysis_interval
        self.last_analysis_time = time.time()
        self.known_face_encodings = []
        self.known_face_names = []
        self.known_face_width = known_face_width  # Largeur moyenne du visage humain en mètres
        self.focal_length = focal_length          # Longueur focale en pixels (à ajuster)

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

    def estimate_distance(self, face_width_in_frame):
        # Estimation de la distance en fonction de la largeur du visage dans l'image
        return (self.known_face_width * self.focal_length) / face_width_in_frame

    def face_recognization(self, frame):
        with self.mp_face_detection.FaceDetection(min_detection_confidence=0.5) as face_detection:
            frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
            results = face_detection.process(frame_rgb)

            face_data = []  # Contiendra les distances et positions des visages

            if results.detections:
                for detection in results.detections:
                    self.mp_drawing.draw_detection(frame, detection)

                    bboxC = detection.location_data.relative_bounding_box
                    h, w, _ = frame.shape
                    top = int(bboxC.ymin * h)
                    left = int(bboxC.xmin * w)
                    bottom = int((bboxC.ymin + bboxC.height) * h)
                    right = int((bboxC.xmin + bboxC.width) * w)

                    # Calculer la largeur du visage en pixels et estimer la distance
                    face_width_in_frame = right - left
                    distance = self.estimate_distance(face_width_in_frame)

                    # Enregistrer les données de position et de distance du visage
                    face_data.append({
                        "distance": distance,
                        "x1": left,
                        "y1": top,
                        "x2": right,
                        "y2": bottom
                    })

                    # Afficher la distance sur l'image
                    cv2.putText(frame, f"{distance:.2f} m", (left, bottom + 20),
                                cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 0, 0), 2)
            else:
                variables.names = []

            # Effectuer la reconnaissance toutes les `analysis_interval` secondes
            current_time = time.time()
            if current_time - self.last_analysis_time >= self.analysis_interval and face_data:
                face_encodings = fr.face_encodings(frame_rgb, [(d["y1"], d["x2"], d["y2"], d["x1"]) for d in face_data])
                variables.names = []

                for (face_dict, face_encoding) in zip(face_data, face_encodings):
                    matches = fr.compare_faces(self.known_face_encodings, face_encoding)
                    name = "Inconnu"
                    face_distances = fr.face_distance(self.known_face_encodings, face_encoding)
                    best_match_index = face_distances.argmin()
                    if matches[best_match_index]:
                        name = self.known_face_names[best_match_index]
                    
                    # Ajouter le nom reconnu aux données de visage
                    face_dict["name"] = name
                    variables.names.append(name)

                # Mettre à jour le temps de la dernière analyse
                self.last_analysis_time = current_time

        return frame, face_data