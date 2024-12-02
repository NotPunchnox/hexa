const express = require('express');
const app = express();

app.use(express.json());

module.exports = () => {
    app.get('/', (req, res) => {
        res.send('Api lancé')
    })

    app.post("/cmd", (req, res) => {
        let { ws } = require('../vars/index');
        if (!req.body.animations || req.body.animations.length <= 0) {
            return res.status(400).send("Format incorrect!");
        }
        
        console.log(req.body.animations);

        if (!ws || ws.readyState !== 1) {
            return res.status(500).send("WebSocket non connecté.");
        }

        try {
            ws.send(JSON.stringify({ animations: req.body.animations }));
            res.status(200).send("Animations envoyées avec succès!");
        } catch (error) {
            console.error("Erreur lors de l'envoi des données via WebSocket:", error);
            res.status(500).send("Erreur lors de l'envoi des données.");
        }
    });

    app.listen(3001, () => {
        console.log('Serveur démarré sur le port 3001');
    });
};
