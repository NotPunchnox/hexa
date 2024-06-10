const fs = require('fs');
const http = require('http');
const { cocoSsd } = require('@tensorflow-models/coco-ssd');
const tf = require('@tensorflow/tfjs-node');

// Fonction pour détecter les objets dans une image
async function detectObjects(imageBuffer) {
    // Charger le modèle COCO-SSD
    const model = await cocoSsd.load();

    // Prétraitement de l'image
    const inputImage = tf.node.decodeImage(imageBuffer);
    
    // Prédiction
    const predictions = await model.detect(inputImage);

    return predictions;
}

// Fonction pour lire le flux vidéo à partir de l'URL
function readVideoStream(url) {
    const server = http.createServer((req, res) => {
        // Faire une demande HTTP pour récupérer le flux vidéo
        http.get(url, (streamRes) => {
            let data = Buffer.from([]);
            streamRes.on('data', (chunk) => {
                data = Buffer.concat([data, chunk]);
            });
            streamRes.on('end', async () => {
                try {
                    // Détecter les objets dans l'image
                    const objects = await detectObjects(data);
                    console.log('Objects detected:', objects);
                } catch (error) {
                    console.error('Error detecting objects:', error);
                }
            });
        });
    });

    // Démarrer le serveur
    const port = 3000;
    server.listen(port, () => {
        console.log(`Server running on port ${port}`);
    });
}

// Appel de la fonction pour lire le flux vidéo
const videoUrl = 'http://192.168.1.52:8000/stream.mjpg';
readVideoStream(videoUrl);
