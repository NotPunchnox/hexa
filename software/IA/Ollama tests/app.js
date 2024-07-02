const axios = require('axios');
const fs = require('fs');
const path = require('path');
const cosineSimilarity = require('cosine-similarity');
const prompt = require('prompt-sync')();

const conversationFile = path.join(__dirname, './models/model-format.json');
const embeddingsFile = path.join(__dirname, './models/embeddings.json');
const embeddingActionFile = path.join(__dirname, './models/embeddings-action.json');
const actionExamplesFile = path.join(__dirname, './models/model-format-actions.json');

let conversation = [];
if (fs.existsSync(conversationFile)) {
    conversation = JSON.parse(fs.readFileSync(conversationFile, 'utf-8'));
}
let embeddings = [];
if (fs.existsSync(embeddingsFile)) {
    embeddings = JSON.parse(fs.readFileSync(embeddingsFile, 'utf-8'));
}
let embeddingsAction = [];
if (fs.existsSync(embeddingActionFile)) {
    embeddingsAction = JSON.parse(fs.readFileSync(embeddingActionFile, 'utf-8'));
}
let actionExamples = [];
if (fs.existsSync(actionExamplesFile)) {
    actionExamples = JSON.parse(fs.readFileSync(actionExamplesFile, 'utf-8'));
}

async function getEmbedding(prompt) {
    const response = await axios.post('http://127.0.0.1:11434/api/embeddings', {
        model: 'nomic-embed-text',
        prompt: prompt
    });
    return response.data.embedding;
}

function findMostRelevantDocs(embedding, embeddings, documents, n = 3) {
    if (!embeddings.length) {
        return ["Aucune conversation précédente trouvée."];
    }

    const similarities = embeddings.map(e => cosineSimilarity(embedding, JSON.parse(e)));
    const sortedIndices = similarities.map((sim, index) => [sim, index])
                                      .sort((a, b) => b[0] - a[0])
                                      .slice(0, n)
                                      .map(pair => pair[1]);
    return sortedIndices.map(index => documents[index]);
}

(async () => {
    while (true) {
        const promptText = prompt('\x1b[33mQuestion: \x1b[33m');
        if (promptText === "exit") {
            console.log('Goodbye!');
            process.exit(0);
        }

        const promptEmbedding = await getEmbedding(promptText);
        const promptEmbeddingAction = await getEmbedding(promptText);

        const relevantDocs = findMostRelevantDocs(promptEmbedding, embeddings, conversation);
        const relevantActions = findMostRelevantDocs(promptEmbeddingAction, embeddingsAction, actionExamples);
        const formattedDocs = relevantDocs.map(doc => JSON.stringify(doc)).join(";");
        const formattedActions = relevantActions.map(doc => JSON.stringify(doc)).join(";");

        console.log(`\x1b[35mAnciennes conversations: ${formattedDocs}\x1b[0m`);
        console.log(`\x1b[35mAnciennes actions: ${formattedActions}\x1b[0m`);

        const startTime = Date.now();
        const promptFinal = `Tu es un robot et tu parles avec une personne.
L'interaction te rappelle ces conversations: [${formattedDocs}]
L'interaction te rappelle ces actions: [${formattedActions}]

La question est la suivante: ${promptText}

Important: remplace TOUJOURS ce qui suit le nom de l'action par les numéro et valeurs appropriés.

Ton output en JSON: `;

        const output = await axios.post('http://192.168.1.83:3000/api/generate', {
            model: 'TestQwen',
            prompt: promptFinal,
            stream: false,
            format: 'json'
        });

        const endTime = Date.now();
        const responseTime = (endTime - startTime) / 1000;

        const resultMessage = output.data.response;
        console.log(resultMessage);
        console.log(`\x1b[32mTemps de génération de la réponse: ${responseTime.toFixed(2)} secondes\x1b[0m`);

        const combinedMessage = { input: promptText, output: resultMessage };
        conversation.push(JSON.stringify(combinedMessage));

        fs.writeFileSync(conversationFile, JSON.stringify(conversation, null, 4), 'utf-8');

        const newEmbedding = await getEmbedding(JSON.stringify(combinedMessage));
        embeddings.push(JSON.stringify(newEmbedding));
        const newEmbeddingAction = await getEmbedding(JSON.stringify(combinedMessage));
        embeddingsAction.push(JSON.stringify(newEmbeddingAction));

        fs.writeFileSync(embeddingsFile, JSON.stringify(embeddings, null, 4), 'utf-8');
        fs.writeFileSync(embeddingActionFile, JSON.stringify(embeddingsAction, null, 4), 'utf-8');
    }
})();
