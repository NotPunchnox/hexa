import speech_recognition as sr
from google.cloud import texttospeech
import requests
import pygame
import json
import time
import os

# Initialisation de pygame pour jouer le son
pygame.mixer.init()
client = texttospeech.TextToSpeechClient()


def recognize_speech_from_mic(recognizer, microphone):
    with microphone as source:
        print("Ajustement pour le bruit ambiant...")
        recognizer.adjust_for_ambient_noise(source)
        print("Écoute...")
        audio = recognizer.listen(source)
    
    response = {
        "success": True,
        "error": None,
        "transcription": None
    }

    try:
        response["transcription"] = recognizer.recognize_google(audio, language="fr-FR")
    except sr.RequestError:
        response["success"] = False
        response["error"] = "API indisponible"
    except sr.UnknownValueError:
        response["error"] = "Impossible de reconnaître la parole"
    
    return response

# Fonction pour envoyer le texte à l'API et obtenir la réponse
def send_request_to_api(content):
    url = "http://localhost:3000/api/question"
    data = {"content": content}
    headers = {"Content-Type": "application/json"}

    response = requests.get(url, data=json.dumps(data), headers=headers)
    print(response)
    return response.json()

# Fonction pour convertir le texte en parole et jouer le son
def speak_text(text):
    synthesis_input = texttospeech.SynthesisInput(text=text)
    voice = texttospeech.VoiceSelectionParams(
        language_code="fr-FR", ssml_gender=texttospeech.SsmlVoiceGender.NEUTRAL, name="fr-FR-Wavenet-B",
    )
    audio_config = texttospeech.AudioConfig(
        audio_encoding=texttospeech.AudioEncoding.MP3
    )
    response = client.synthesize_speech(
        input=synthesis_input, voice=voice, audio_config=audio_config
    )
    with open("response.mp3", "wb") as out:
        out.write(response.audio_content)
        print('Le contenu de l\'auto a été sauvegardé dans le fichier: "response.mp3"')
    
    pygame.mixer.music.load("response.mp3")
    pygame.mixer.music.play()
    
    while pygame.mixer.music.get_busy():
        time.sleep(1)
    
    pygame.mixer.music.stop()
    pygame.mixer.music.unload()
    os.remove("response.mp3")


# Fonction main
def main():

    while True:
    
        recognizer = sr.Recognizer()
        microphone = sr.Microphone()

        print("Parlez maintenant!")
        speech_recognition_result = recognize_speech_from_mic(recognizer, microphone)

        if speech_recognition_result["transcription"]:
            print("Vous avez dit: {}".format(speech_recognition_result["transcription"]))
            api_response = send_request_to_api(speech_recognition_result["transcription"])
            
            # Extraire les informations nécessaires de la réponse de l'API
            try:
                content = json.loads(api_response["content"])
            except:
                content = api_response["content"]

            sauvegarde = content["Sauvegarde"]
            actions = content["actions"]
            message = content["message"]

            if len(actions) > 0:
                actions = "actions à effectuer: " + ', '.join(actions)
            else:
                actions = "aucune action à effectuer"
        
            final_response = f"Expérimentation numéro: {sauvegarde}, {actions}, {message}"
            
            print(final_response)
            speak_text(final_response)
            
            
        elif speech_recognition_result["error"]:
            print("ERREUR: {}".format(speech_recognition_result["error"]))
        else:
            print("Je n'ai pas compris. Pouvez-vous répéter?")

if __name__ == "__main__":
    main()