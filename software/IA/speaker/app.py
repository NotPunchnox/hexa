import speech_recognition as sr
from google.cloud import texttospeech
from os import environ
import requests, pygame, json, time, os, serial

pygame.mixer.init()
environ['PYGAME_HIDE_SUPPORT_PROMPT'] = '1'

client = texttospeech.TextToSpeechClient()
#ser = serial.Serial('COM11', 115200)
ser = ""


def recognize_speech_from_mic(recognizer, microphone):
    with microphone as source:
        print("\033[43mAjustement pour le bruit ambiant...\033[0m")
        recognizer.adjust_for_ambient_noise(source, duration=1)
        print("\033[32mÉcoute...\033[0m")
        audio = recognizer.listen(source, timeout=5, phrase_time_limit=10)
    
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
    url = "http://localhost:3001/api/question"
    data = {"content": content}
    headers = {"Content-Type": "application/json"}

    response = requests.post(url, data=json.dumps(data), headers=headers)
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
        print('\033[34mLe contenu de l\'auto a été sauvegardé dans le fichier: "response.mp3"\033[0m')
    
    pygame.mixer.music.load("response.mp3")
    pygame.mixer.music.play()
    
    while pygame.mixer.music.get_busy():
        time.sleep(1)
    
    pygame.mixer.music.stop()
    pygame.mixer.music.unload()
    os.remove("response.mp3")



#envoi de réponse sur le port com série
def send_command(command):
    
    ser.write((command + '\n').encode())
    time.sleep(1)

    response = ser.read_all().decode(errors='ignore').strip()
    print("Hardware:", response)
    return response

# Fonction main
def main():
    os.system('cls')

    Mode = input("Voice or chat ? \nRépondez par [voice\chat]: ")
    Dev = input("\nMode Développeur [Y\\N]:")

    while True:

        if Mode == "voice":
            recognizer = sr.Recognizer()
            microphone = sr.Microphone()

            print("\033[43mParlez maintenant!\033[0m")

            speech_recognition_result = recognize_speech_from_mic(recognizer, microphone)
            text = speech_recognition_result["transcription"]

            print("Vous avez dit: {}\033[32m".format(text)+"\033[0m")

        elif Mode == "chat":
            text = input("Vous: ")

        if text:
            api_response = send_request_to_api(text)
            
            # Extraire les informations nécessaires de la réponse de l'API
            try:
                content = json.loads(api_response["content"])
            except:
                content = api_response["content"]

            actions = content["actions"]
            message = content["message"]

            if len(actions) > 0:
                actions = "actions à effectuer: " + ', '.join(actions)
            else:
                actions = "aucune action à effectuer"
        
            final_response = f"{message}"

            if Dev == "N":
                for command in content["actions"]:
                    send_command(command)
            speak_text(final_response)
            
            print("Aash: \033[36m"+ final_response+"\033[0m")
            print("Aash: \033[36m"+ actions +"\033[0m")
            
        elif speech_recognition_result["error"]:
            print("ERREUR: {}".format(speech_recognition_result["error"]))
        else:
            print("Je n'ai pas compris. Pouvez-vous répéter?")

if __name__ == "__main__":
    main()