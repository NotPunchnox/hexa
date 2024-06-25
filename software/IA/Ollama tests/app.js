const request = require('request');
const fs = require('fs');
const readline = require('readline');

let chatHistory = require('./models/chatHistory.json');

const saveChatHistory = () => {
    fs.writeFileSync('./models/chatHistory.json', JSON.stringify(chatHistory, null, 2), 'utf8');
};

const getBotResponse = () => {
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
                messages: chatHistory
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

const startChat = () => {
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
            const botResponse = await getBotResponse();

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

startChat();
