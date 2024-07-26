import express from "express";
const app = express();

/* Configuration */
const PORT = 8000

app.use(express.json());

app.post('/api/vision', (req, res) => {

    if(!req.body) return res.status(400).json({code: 400, message: 'bad request'})
    console.log(req.body)

    return res.status(201).json({code: 201, message: req.body})
})

app.listen(PORT, () => {
    console.log('Serveur en cours sur le port:', PORT);
});