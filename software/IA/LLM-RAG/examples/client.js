import config from "../config.json" assert { type: "json" }
import {ChatHistory} from "../src/router/Question.js"
import fs from "fs"
import {getVectorStore} from "../src/controller/embedding.js"

export default config

import readline from "readline";
import InitLogic from "../src/controller/InitLogic.js";
import Question from "../src/router/Question.js";
import { AddDoc } from "../src/controller/initEmbedding.js";
import path from "path";

const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout,
  prompt: '\x1b[33m\x1b[1mQuestion:\x1b[0m '
});

const initClient = async () => {
  try {
    await InitLogic();
    console.log('\x1b[32mInterface initialisée avec succès.\x1b[0m\n');
  } catch (error) {
    console.error('\x1b[31mErreur lors de l\'initialisation de l\'interface:', error, '\x1b[0m');
    process.exit(1);
  }
};


const chatLoop = async () => {
  rl.prompt();

  rl.on('line', async (line) => {
    let prompt = line.trim();

    if (prompt.toLowerCase() === 'exit') {
      console.log('\x1b[32mSession terminée.\x1b[0m');
      rl.close();
      return;
    }

    try {
      let modelSelected = config.LLM_MODEL;
      let question = prompt;
      if(prompt.startsWith('config') && prompt.split(' ').length > 1) {
        if(prompt.split(' ')[1] == "tts") config.tts = Boolean(prompt.split(' ')[2]);
        if(prompt.split(' ')[1] == "model") config.LLM_MODEL = prompt.split(' ')[2];
        if(prompt.split(' ')[1] === "format") config.format = prompt.split(' ')[2] !== "json" ? null : "json";
        if(prompt.split(' ')[1] === "stream") config.format = Boolean(prompt.split(' ')[2]);
        return console.log('Saved !')
      }
      if(prompt === "help") return console.log("- config tts (true|false)\n- config model (gemma)\n- config format (null|json)\n- config stream (true|false)\n- chat: get chat History\n- clear|cls: clear console")
      if(prompt === "chat") return console.log('\n', ChatHistory, '\n');
      if(prompt === "cls" || prompt === "clear") return console.clear();

      // if (prompt.includes('-')) {
      //     const parts = prompt.split('-');
      //     question = parts[0];
      //     modelSelected = parts[1] || null;
      // }

      const {result} = await Question(question, 2, modelSelected);
      if(!config.stream) console.log('\n\x1b[1mRéponse:\x1b[0m\x1b[36m', result, '\x1b[0m\n');

      const vectorStore = await getVectorStore();

      /** Save In VectorStore */
      let now = new Date();
      let p = path.resolve(`./src/Training Data/conversations/conv-${now.getFullYear()}-${('0' + (now.getMonth() + 1)).slice(-2)}-${('0' + now.getDate()).slice(-2)}-${('0' + now.getHours()).slice(-2)}.json`);
      
      AddDoc([`user: ${question}`, `IA: ${JSON.stringify(result)}`], vectorStore.vectorStoreConversations);

      if(!fs.existsSync(p)) fs.writeFileSync(p, '[]')
      let data = fs.readFileSync(p, 'utf-8')
      data = JSON.parse(String(data));
      data.push(`user: ${question}; IA: ${JSON.stringify(result)}`)
      
      fs.writeFileSync(p, JSON.stringify(data), 'utf-8')
      
    } catch (error) {
      console.error('\x1b[31mErreur lors de la récupération de la réponse:', error, '\x1b[0m');
    }

    rl.prompt();
  }).on('close', () => {
    console.log('\x1b[32mAu revoir!\x1b[0m');
    process.exit(0);
  });
};


const main = async () => {
  await initClient();
  chatLoop();
};

main().catch(error => {
  console.error('\x1b[31mErreur lors de l\'exécution du client:', error, '\x1b[0m');
});
