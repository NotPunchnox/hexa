// Connexion WebSocket
const socket = new WebSocket('ws://192.168.1.23:8080'); // Remplacer par l'URL de votre serveur WebSocket
socket.onopen = () => console.log('Connexion WebSocket établie');
socket.onerror = (error) => console.error('Erreur WebSocket:', error);
socket.onclose = () => console.log('Connexion WebSocket fermée');

// Variables pour la gestion des joysticks
let joystickMode = "ChangeXY"; // Initialisation du mode de joystick
const joystickModeButton = document.getElementById("joystick-mode");
const changeTopSlider = document.getElementById("change-top-slider");
const changeTopValue = document.getElementById("change-top-value");

let currentHeight = 0;
let lastSentCommand = ""; // Pour éviter les commandes en double
let isDragging = false; // Si le joystick est en mouvement

// Fonction pour envoyer une commande via WebSocket
function sendCommand(command) {
    if (command !== lastSentCommand) {
        socket.send(command);
        lastSentCommand = command;
        console.log('Commande envoyée:', command);
    }
}

// Fonction pour changer le mode du joystick (ChangeXY / Rouli)
joystickModeButton.addEventListener('click', () => {
    joystickMode = joystickMode === 'ChangeXY' ? 'Rouli' : 'ChangeXY';
    joystickModeButton.textContent = `Mode: ${joystickMode}`;
});

// Fonction pour mettre à jour la hauteur avec le slider
changeTopSlider.addEventListener('input', () => {
    currentHeight = parseInt(changeTopSlider.value);
    changeTopValue.textContent = `Hauteur: ${currentHeight}`;
    sendCommand(`ChangeTop_4_${currentHeight}`);
});

// Fonction pour gérer les mouvements des joysticks
function handleJoystickMove(joystick, stick, mode) {
    let startX = 0, startY = 0;

    joystick.addEventListener('mousedown', (e) => {
        isDragging = true;
        const rect = joystick.getBoundingClientRect();
        startX = e.clientX - rect.left;
        startY = e.clientY - rect.top;
    });

    joystick.addEventListener('mouseup', () => {
        isDragging = false;
        stick.style.left = '50%';
        stick.style.top = '50%';
        sendCommand("Stop");
    });

    joystick.addEventListener('mousemove', (e) => {
        if (!isDragging) return;

        const rect = joystick.getBoundingClientRect();
        const deltaX = e.clientX - rect.left - startX;
        const deltaY = e.clientY - rect.top - startY;

        const maxDistance = rect.width / 2;
        const distanceX = Math.min(Math.max(deltaX, -maxDistance), maxDistance);
        const distanceY = Math.min(Math.max(deltaY, -maxDistance), maxDistance);

        stick.style.left = `50% + ${distanceX}px`;
        stick.style.top = `50% + ${distanceY}px`;

        const normalizedX = Math.round((distanceX / maxDistance) * 5);
        const normalizedY = Math.round((distanceY / maxDistance) * -5);

        if (Math.abs(normalizedX) < 1) normalizedX = 0;
        if (Math.abs(normalizedY) < 1) normalizedY = 0;

        const command =
            mode === 'ChangeXY'
                ? `ChangeXY_4_${normalizedX}_${normalizedY}`
                : `Rouli_4_${normalizedX}_${normalizedY}`;
        
        sendCommand(command);
    });
}

// Initialisation des joysticks
const joystickLeft = document.getElementById("joystick-left");
const stickLeft = document.getElementById("stick-left");

handleJoystickMove(joystickLeft, stickLeft, "ChangeXY"); // Joystick de gauche en mode ChangeXY

// Fonction pour changer le mode du joystick
const joystickRight = document.getElementById("joystick-right");
const stickRight = document.getElementById("stick-right");

handleJoystickMove(joystickRight, stickRight, "Rouli"); // Joystick de droite en mode Rouli
