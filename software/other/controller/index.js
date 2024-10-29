const VARS = require('./src/vars/index.js');
const WebSocket = require('ws');
const rl = require('readline-sync');

const ws = new WebSocket('ws://192.168.1.23:8080/'); // ws://<raspberry_ip>:<port_attribué>/
ws.on('error', console.error);

ws.on('open', function open() {
    console.log('Connexion WebSocket établie.');
    promptUser();

    setInterval(() => ws.send(JSON.stringify({})), 10000);
});

const promptUser = () => {
    let animations = [];
    let prompt = rl.question('Commande: ');

    if (prompt.length < 1) return;  // Si la commande est vide, ne rien faire

    if (prompt === 'clear') {
        console.clear();
        return promptUser();  // Appelle la fonction à nouveau après avoir effacé l'écran
    }

    if (prompt.includes(',')) {
        let parse = prompt.replace(/ /g, '').split(',');
        animations = parse;
        console.log(animations)
    } else {
        animations.push(prompt);
    }

    // Envoi de la commande au serveur WebSocket
    ws.send(JSON.stringify({ animations }));
    console.log("Commande envoyée !");

    // Appel récursif pour continuer à demander des commandes
    setTimeout(promptUser, 200); // Attendre un court instant avant de demander à nouveau
};

ws.on('message', function message(data) {
    console.log('Message reçu: %s', data);
});

console.log('vars', VARS);

