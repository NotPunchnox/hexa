import express from 'express';
import bodyParser from 'body-parser';
import config from "../config.json" assert { type: "json" }
import fs from "fs"
import {getVectorStore} from "../src/controller/embedding.js"

import InitLogic from "../src/controller/InitLogic.js";
import Question, {ChatHistory} from "../src/router/Question.js";
import { AddDoc } from "../src/controller/initEmbedding.js";
import path from "path";
import WebSocket from "ws"

// Initialiser l'application Express
const app = express();
const PORT = process.env.PORT || 3000;

// Middleware pour parser les JSON
app.use(bodyParser.json());

// Exécution de l'initialisation
try {
    await InitLogic();
    console.log('\x1b[32mInterface initialisée avec succès.\x1b[0m\n');
  } catch (error) {
    console.error('\x1b[31mErreur lors de l\'initialisation de l\'interface:', error, '\x1b[0m');
    process.exit(1);
}

const ws = new WebSocket("ws://" + config.RASPBERRY_PI_ZERO_IP + ':' + config.PORT_WEBSOCKET); // ws://<raspberry_ip>:<port_attribué>/
ws.on('error', console.error);
    
ws.on('open', function open() {
    console.log('Connexion WebSocket établie.');
    setInterval(() => {
        if (ws.readyState === WebSocket.OPEN) {
            ws.send("ping");
            console.log("ping envoyé");
        }
    }, 5000);

    // Endpoint pour envoyer une question
    app.post('/ask', async (req, res) => {
      const { question, model } = req.body;

      if (!question) {
        return res.status(400).json({ error: 'Question is required.' });
      }

      try {
        let modelSelected = model ? model : config.LLM_MODEL;
        const { result } = await Question(question, 2, modelSelected);

        console.log("actions:", result.actions, "result:", result);
        if (result.actions.length > 0) ws.send(JSON.stringify({animations: result.actions}));
        res.json({ response: result });

        const vectorStore = await getVectorStore();

        // Enregistrer dans VectorStore
        let now = new Date();
        let p = path.resolve(`./src/Training Data/conversations/conv-${now.getFullYear()}-${('0' + (now.getMonth() + 1)).slice(-2)}-${('0' + now.getDate()).slice(-2)}-${('0' + now.getHours()).slice(-2)}.json`);

        AddDoc([`user: ${question}`, `IA: ${JSON.stringify(result)}`], vectorStore.vectorStoreConversations);

        if (!fs.existsSync(p)) fs.writeFileSync(p, '[]');
        let data = fs.readFileSync(p, 'utf-8');
        data = JSON.parse(String(data));
        data.push(`user: ${question}; IA: ${JSON.stringify(result)}`);

        fs.writeFileSync(p, JSON.stringify(data), 'utf-8');

      } catch (error) {
        console.error('\x1b[31mErreur lors de la récupération de la réponse:', error, '\x1b[0m');
        res.status(500).json({ error: 'Erreur lors de la récupération de la réponse.' });
      }
    });

});

app.get('/history' , (req, res) => {
    return res.status(200).json({ result: ChatHistory })
})

// Démarrer le serveur
app.listen(PORT, () => {
    console.log(`Server is running on port ${PORT}`);
});
