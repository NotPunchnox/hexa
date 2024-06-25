const fs = require('fs');
const readline = require('readline');
const request = require('request');
const express = require('express');

const app = express();
const port = 3000;

let chatHistory = require('./models/chatHistory.json');

const saveChatHistory = () => {
    fs.writeFileSync('./models/chatHistory.json', JSON.stringify(chatHistory, null, 2), 'utf8');
};

const getBotResponse = (messages) => {
    return new Promise((resolve, reject) => {
        const options = {
            method: 'POST',
            url: 'http://localhost:11434/api/chat',
            headers: {
                'Content-Type': 'application/json',
                'User-Agent': 'insomnia/2023.5.8'
            },
            body: {
                stream: false,
                json: true,
                model: 'Test',
                messages: messages
            },
            json: true
        };

        request(options, (error, response, body) => {
            if (error) {
                return reject(error);
            }
            resolve(body.message.content);
        });
    });
};

const startChatCLI = () => {
    console.log('Welcome to the chat with the bot! Type "exit" to end the conversation.');

    const rl = readline.createInterface({
        input: process.stdin,
        output: process.stdout
    });

    rl.on('line', async (userMessage) => {
        if (userMessage.toLowerCase() === 'exit') {
            rl.close();
            return;
        }

        chatHistory.push({
            role: 'user',
            content: userMessage
        });

        try {
            const botResponse = await getBotResponse(chatHistory);

            chatHistory.push({
                role: 'assistant',
                content: botResponse
            });

            console.log(`\x1b[34mBot: \x1b[36m${JSON.parse(botResponse).message} ${JSON.parse(botResponse).actions}\x1b[0m`);

            saveChatHistory();
        } catch (error) {
            console.error('Error communicating with the bot:', error);
        }
    });

    rl.on('close', () => {
        console.log('Goodbye!');
        saveChatHistory();
        process.exit(0);
    });
};

const startAPI = () => {
    app.use(express.json());

    app.post('/chat', async (req, res) => {
        const userMessage = req.body.message;

        chatHistory.push({
            role: 'user',
            content: userMessage
        });

        try {
            const botResponse = await getBotResponse(chatHistory);

            chatHistory.push({
                role: 'assistant',
                content: botResponse
            });

            //saveChatHistory();

            res.json({
                message: JSON.parse(botResponse)
            });
        } catch (error) {
            console.error('Error communicating with the bot:', error);
            res.status(500).json({ error: 'Error communicating with the bot' });
        }
    });

    app.listen(port, () => {
        console.log(`API server running at http://localhost:${port}/`);
    });
};

const startApp = (mode) => {
    if (mode === 'api') {
        startAPI();
    } else if (mode === 'chat') {
        startChatCLI();
    } else {
        console.error('Invalid mode. Please choose "api" or "chat".');
        process.exit(1);
    }
};

// Choix du mode : "api" ou "chat"
const mode = process.argv[2];

startApp(mode);
