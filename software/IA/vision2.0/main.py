import cv2, time
from controllers.extract_text.find_text import find_text
from controllers.segmentation.Object_detection import Object_detection
from controllers.Face.face_detector import FaceRecognition

Face = FaceRecognition()

# Ouverture de la caméra
webcam = cv2.VideoCapture(0)
#webcam = cv2.VideoCapture("http://192.168.1.23:8000/stream.mjpg")

width = int(webcam.get(cv2.CAP_PROP_FRAME_WIDTH))
height = int(webcam.get(cv2.CAP_PROP_FRAME_HEIGHT))
print(f"Dimensions de la caméra : largeur = {width}, hauteur = {height}")

frame_counter = 0
#webcam.set(cv2.CAP_PROP_FPS, 15)

while ( webcam.isOpened() ):
    # Lire la caméra
    ret, frame = webcam.read()
    if not ret:
        break
    start_time = time.time()
    
    #redimentionner la frame
    frame2 = cv2.resize(frame, (320, 240))
    
    # frame_counter += 1
    # if frame_counter % 5 == 0:

    frame = Object_detection(cv2, frame)
    frame = Face.face_recognization(frame)

    
    # Afficher le résultat en temps réel dans la fenêtre
    end_time = time.time()
    fps = 1 / float(end_time - start_time)
    cv2.putText(frame, f"FPS: {fps:.2f}", (240, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 255), 2)

    cv2.imshow("Aash Vision; by punchnox", frame)
    
    # Arret du programme avec la touche q
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

webcam.release()
cv2.destroyAllWindows()

#Crédit : github.com/notpunchnox/hexa
