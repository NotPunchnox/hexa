const {
    GoogleGenerativeAI,
    HarmCategory,
    HarmBlockThreshold
} = require("@google/generative-ai");
const fs = require('fs');
const {
    history
} = require('./Modal.json');
require('dotenv').config();

const apiKey = process.env.API_KEY;
const genAI = new GoogleGenerativeAI(apiKey);

console.log(genAI.getGenerativeModel)

const model = genAI.getGenerativeModel({
    model: "gemini-1.5-flash",
});

const generationConfig = {
    temperature: 1,
    topP: 0.95,
    topK: 64,
    maxOutputTokens: 8192,
    responseMimeType: "application/json",
};

const safetySettings = [{
        category: HarmCategory.HARM_CATEGORY_HARASSMENT,
        threshold: HarmBlockThreshold.BLOCK_MEDIUM_AND_ABOVE,
    },
    {
        category: HarmCategory.HARM_CATEGORY_HATE_SPEECH,
        threshold: HarmBlockThreshold.BLOCK_MEDIUM_AND_ABOVE,
    },
    {
        category: HarmCategory.HARM_CATEGORY_SEXUALLY_EXPLICIT,
        threshold: HarmBlockThreshold.BLOCK_MEDIUM_AND_ABOVE,
    },
    {
        category: HarmCategory.HARM_CATEGORY_DANGEROUS_CONTENT,
        threshold: HarmBlockThreshold.BLOCK_MEDIUM_AND_ABOVE,
    },
];

function Train(text, result) {
    let aa = [];
    aa.push({
        role: "user",
        parts: [{
            text: text
        }],
    }, {
        role: "model",
        parts: [{
            text: result
        }],
    });

    console.log('Données sauvegardées');
    fs.writeFileSync(__dirname + '/Modal.json', JSON.stringify({ history }, null, 2));
}

async function run(text) {
    const chatSession = model.startChat({
        generationConfig,
        safetySettings,
        history: history,
    });
    const result = await chatSession.sendMessage(text);
    return {
        result,
        text
    };
}

module.exports = {
    run,
    Train
};