const rl = require('readline-sync');

var ws = null;

const promptUser = websocket => {
    ws = websocket;
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
    setTimeout(promptUser, 200);
};

module.exports = promptUser;