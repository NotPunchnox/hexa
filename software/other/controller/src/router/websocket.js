const WebSocket = require('ws');
const promptUser = require('../controller/promptUser.js')

const mode = process.argv[2];

function websocket() {
    const ws = new WebSocket("ws://" + (!mode ? (process.env.RASPBERRY_PI_ZERO_IP + ':' + process.env.PORT_WEBSOCKET) : (process.env.DEV_RASPBERRY_PI_ZERO_IP + ':' + process.env.DEV_PORT_WEBSOCKET))); // ws://<raspberry_ip>:<port_attribué>/
    ws.on('error', console.error);
    
    ws.on('open', function open() {
        console.log('Connexion WebSocket établie.');
        promptUser(ws);
    
        setInterval(() => ws.send(JSON.stringify({})), 10000);
    });

    ws.on('message', function message(data) {
        console.log('Message reçu: %s', data);
    });
}

module.exports = websocket;