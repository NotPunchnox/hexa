from flask import Flask, request, jsonify
from TTS.api import TTS
import time
import os
import soundfile as sf
import sounddevice as sd

app = Flask(__name__)

# Configuration du modèle TTS
model_name = "tts_models/fr/mai/tacotron2-DDC"
tts = TTS(model_name)

@app.route('/synthesize', methods=['POST'])
def synthesize():
    data = request.get_json()

    if 'phrase' not in data:
        return jsonify({"error": "Aucune phrase fournie."}), 400

    phrase = data['phrase']
    output_file = "output.wav"

    # Temps de génération
    start_time = time.time()
    tts.tts_to_file(text=phrase, file_path=output_file)
    end_time = time.time()

    # Lecture du fichier audio
    data, samplerate = sf.read(output_file)
    sd.play(data, samplerate)
    sd.wait()  # Attend la fin de la lecture

    # Suppression du fichier audio
    os.remove(output_file)

    return jsonify({
        "message": f"Audio généré et joué pour '{phrase}'.",
        "generation_time": end_time - start_time
    })

if __name__ == '__main__':
    app.run(debug=True)
