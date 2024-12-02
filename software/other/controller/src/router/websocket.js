const WebSocket = require('ws');
const promptUser = require('../controller/promptUser.js');
const sharedVars = require('../vars/index.js');

const mode = process.argv[2];

function websocket() {
    const wsUrl = "ws://" + (!mode ? 
        `${process.env.RASPBERRY_PI_ZERO_IP}:${process.env.PORT_WEBSOCKET}` : 
        `${process.env.DEV_RASPBERRY_PI_ZERO_IP}:${process.env.DEV_PORT_WEBSOCKET}`);
    
    console.log("Serveur WebSocket:", wsUrl);
    
    sharedVars.ws = new WebSocket(wsUrl);
    
    sharedVars.ws.on('error', (error) => {
        console.error("Erreur WebSocket:", error);
        setTimeout(websocket, 5000);
    });

    sharedVars.ws.on('open', function open() {
        console.log('Connexion WebSocket établie.');

        const pingInterval = setInterval(() => {
            if (sharedVars.ws.readyState === WebSocket.OPEN) {
                sharedVars.ws.send("ping");
                console.log("ping envoyé");
            } else {
                clearInterval(pingInterval);
            }
        }, 5000);

        // promptUser();
    });

    sharedVars.ws.on('close', () => {
        console.log("Connexion WebSocket fermée, tentative de reconnexion...");
        setTimeout(websocket, 5000);
    });

    sharedVars.ws.on('message', function message(data) {
        console.log('Message reçu: %s', data);
    });
}

module.exports = websocket;
