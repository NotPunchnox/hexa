const express = require('express');
const app = express();
const IA = require('./src/loadQuestion');
const PORT = 3000;

app.use(express.json());


app.get('/api/question', async(req, res) => {
  if(!req.body.content) return responseIA.status(400).json({code: 400, message: "Bad request: no content"});
  console.log("Nouvelle question:", req.body.content)
  
  var responseIA = await IA.run(req.body.content);
  IA.Train(responseIA.text, responseIA.result.response.text())

  text = responseIA.result.response.text();
  try {
    text = JSON.parse(responseIA.result.response.text());
  } catch(err) {
    console.error(err);
  }

  res.status(201).json({code: 201, content: responseIA.result.response.text()});
})

app.listen(PORT, () => {
  console.log("\x1b[32mLe serveur est en route sur le port: ", PORT, "\x1b[0m");
})