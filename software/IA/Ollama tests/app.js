const axios = require('axios');
const fs = require('fs');
const path = require('path');
const cosineSimilarity = require('cosine-similarity');
const prompt = require('prompt-sync')();
const express = require('express')
const app = express()
let history = require('./models/chatHistory.json')

const Model = "Aash"

const conversationFile = path.join(__dirname, './models/model-format.json');
const embeddingsFile = path.join(__dirname, './models/embeddings.json');
const embeddingActionFile = path.join(__dirname, './models/embeddings-action.json');
const actionExamplesFile = path.join(__dirname, './models/model-format-actions.json');
app.use(express.json());

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

if(process.argv[2] === "reset") {
    fs.writeFileSync(embeddingsFile, "[]", 'utf-8');
    fs.writeFileSync(embeddingActionFile, "[]", 'utf-8');
    (async() => {
        for(i = 0; i<conversationFile.length;i++) {
            const newEmbedding = await getEmbedding(conversationFile[i]);
            embeddings.push(newEmbedding);
            
            fs.writeFileSync(embeddingsFile, JSON.stringify(embeddings, null, 4), 'utf-8');
        }
        for(i = 0; i<embeddingActionFile.length;i++) {
            const newEmbedding = await getEmbedding(embeddingActionFile[i]);
            embeddingsAction.push(newEmbedding);
            
            fs.writeFileSync(embeddingActionFile, JSON.stringify(embeddingsAction, null, 4), 'utf-8');
        }
    })()
}

async function getEmbedding(prompt) {
    const response = await axios.post('http://127.0.0.1:3000/api/embeddings', {
        model: 'nomic-embed-text',
        prompt: prompt
    });
    return response.data.embedding;
}

function findMostRelevantDocs(embedding, embeddings, documents, n = 3) {
    if (!embeddings.length) {
        return ["Aucune conversation précédente trouvée."];
    }

    const similarities = embeddings.map(e => cosineSimilarity(embedding, e));
    const sortedIndices = similarities.map((sim, index) => [sim, index])
        .sort((a, b) => b[0] - a[0])
        .slice(0, n)
        .map(pair => pair[1]);
    return sortedIndices.map(index => documents[index]);
}
function containsMajorityWords(phrase, words, action) {
    const lowerCasePhrase = phrase.toLowerCase();
    const wordCount = words.length;
    let matchCount = 0;

    // Vérification des mots du prompt dans la phrase
    words.forEach(word => {
        if (lowerCasePhrase.includes(word.toLowerCase())) {
            matchCount++;
        }
    });

    // Vérifie si plus de 80% des mots du prompt sont présents
    if ((matchCount / wordCount) >= 0.7) {
        return true;
    }

    // Vérification supplémentaire pour les actions si action est true
    if (action) {
        const wordToPhrase = words.join(' ').toLowerCase();
        const actionKeywords = phrase.split(':')[0].split('|').map(keyword => keyword.toLowerCase());
        actionKeywords.forEach(keyword => {
            console.log('keyword', keyword, wordToPhrase)
            if (wordToPhrase.includes(keyword)) {
                matchCount++;
            }
            console.log(matchCount)
        });

        // Vérifie si plus de 60% des mots clés d'action sont présents
        if ((matchCount / actionKeywords.length) >= 0.7) {
            return true;
        }
    }

    return false;
}


app.post('/api/question', async (req, res) => {
    if (!req.body.content) return res.status(400).json({
        code: 400,
        message: 'bad request'
    });
    const promptText = req.body.content;
    if (promptText === "exit") {
        console.log('Goodbye!');
        process.exit(0);
    }

    let formattedDocs = [];
    let formattedActions = [];
    let relevantDocs;
    let relevantActions;
    const promptWords = promptText.toLowerCase().split(' ');

    for (i = 0; i<conversation.length; i++) {
        if (containsMajorityWords(conversation[i], promptWords)) {
            formattedDocs.push(conversation[i]);
        }
    }
    for (i = 0; i<actionExamples.length; i++) {
        if (containsMajorityWords(actionExamples[i], promptWords, true)) {
            formattedActions.push(actionExamples[i].split(':')[1]);
        }
    }
    if(formattedDocs.length <= 0) {
        const promptEmbedding = await getEmbedding(JSON.stringify({input: promptText, output: ""}));
        relevantDocs = findMostRelevantDocs(promptEmbedding, embeddings, conversation).slice(0, 1);
        formattedDocs = relevantDocs.map(doc => JSON.stringify(doc)).join(";");
    }
    if(formattedActions.length <= 0) {
        const promptEmbeddingAction = await getEmbedding(promptText);
        relevantActions = findMostRelevantDocs(promptEmbeddingAction, embeddingsAction, actionExamples);
        formattedActions = relevantActions.map(doc => String(doc).split(': ')[1]).join(";");
    }
    
    console.log("Relevant Actions:", formattedActions);
    console.log(`\x1b[35mAnciennes conversations: ${formattedDocs}\x1b[0m`);

    const startTime = Date.now();
    const promptFinal = `Tu es un robot et tu parles avec une personne.

Une question vient d'être posée: ${promptText}.
Cela te rappel ces conversations: [${formattedDocs}];
Proposition d'actions: [${formattedActions}];

Pense à rédiger une nouvelle réponse avec le format imposé et la bonne syntaxe pour les actions si tu dois en faire.

Output: `;

    history.push({
        role: "user",
        content: promptFinal
    })

    const output = await axios.post('http://127.0.0.1:3000/api/chat', {
        model: Model,
        messages: history,
        stream: false,
        format: 'json'
    });

    const endTime = Date.now();
    const responseTime = (endTime - startTime) / 1000;

    try {
        JSON.parse(output.data.message.content);
    } catch(err) {
        console.error("Impossible de parse le json", output.data.message.content)
        return res.status(500).json({
            content: {
                actions: [],
                message: "Erreur lors de la récupération de mes actions et de mon texte."
            }
        })
    }
    const resultMessage = JSON.parse(output.data.message.content);
    console.log({
        content: {
            actions: resultMessage.actions || [],
            message: resultMessage.message || ""
        }
    })
    console.log(`\x1b[32mTemps de génération de la réponse: ${responseTime.toFixed(2)} secondes\x1b[0m`);

    //return response
    res.status(200).json({
        content: {
            actions: resultMessage.actions || [],
            message: resultMessage.message || ""
        }
    })

    const combinedMessage = {
        input: promptText,
        output: resultMessage
    };
    conversation.push(JSON.stringify(combinedMessage));

    fs.writeFileSync(conversationFile, JSON.stringify(conversation, null, 4), 'utf-8');

    const newEmbedding = await getEmbedding(JSON.stringify(combinedMessage));
    embeddings.push(newEmbedding);

    fs.writeFileSync(embeddingsFile, JSON.stringify(embeddings, null, 4), 'utf-8');
    fs.writeFileSync(embeddingActionFile, JSON.stringify(embeddingsAction, null, 4), 'utf-8');
})
app.listen(3001, () => {
    console.info('Server listen: http://localhost:3001')
})