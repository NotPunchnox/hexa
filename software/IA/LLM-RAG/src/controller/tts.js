import textToSpeech from "@google-cloud/text-to-speech";
import path from "path";
import fs from "fs";
import util from "util"
import {playAudioFile} from 'audic';

// Creates a client
const client = new textToSpeech.TextToSpeechClient();

export default async(text) => {
  if(!text) return console.error('Impossible de charger le texte !');

  const request = {
    input: {text: text},
    voice: {languageCode: 'fr-FR', name: 'fr-FR-Wavenet-B'},
    audioConfig: {audioEncoding: 'MP3'},
  };

  const [response] = await client.synthesizeSpeech(request);
  const writeFile = util.promisify(fs.writeFile);

  await writeFile(path.resolve('./src/audio/output.mp3'), response.audioContent, 'binary');
  console.log('Audio content written to file: ', path.resolve('./src/audio/output.mp3'));
  await playAudioFile('./src/audio/output.mp3');
}