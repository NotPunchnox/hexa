const express = require('express');
const app = express();
const IA = require('./src/loadQuestion');
const Vision = require('./src/Vision');
const PORT = 8000;

app.use(express.json());

const mode = process.argv[2];
if (mode.toLowerCase() !== 'gemini' && mode.toLowerCase() !== "hexa-neural") {
  return console.clear(''), console.log(mode, ' le model n\'existe pas !')
}


app.get('/api/question', async (req, res) => {
  if (!req.body.content) return res.status(400).json({
    code: 400,
    message: "Bad request: no content"
  });
  console.log("Nouvelle question:", req.body.content)

  if (mode.toLowerCase() == 'gemini') {
    var responseIA = await IA.run(req.body.content);
    IA.Train(responseIA.text, responseIA.result.response.text())

    text = responseIA.result.response.text();
    try {
      text = JSON.parse(responseIA.result.response.text());
    } catch (err) {
      console.error(err);
    }
    res.status(201).json({
      code: 201,
      content: text
    });
  } else {
    require('request').post('http://192.168.1.83:3000/chat', {
      json: true,
      body: {
        message: req.body.content
      }
    }, (err, r, body) => {
      if(err) return console.error(err);

      if(!body) return;
      console.log(body)

      text = body.message
      res.status(201).json({
        code: 201,
        content: text
      });
    })
  }
})


app.get('/api/vision', async (req, res) => {
  if (!req.body.array) return res.status(400).json({
    code: 400,
    message: "Bad request: no content"
  });

  Vision(req.body.array)

  return res.status(201).json({
    code: 201,
    message: "ok"
  })
})

app.listen(PORT, () => {
  console.log("\x1b[32mLe serveur est en route sur le port: ", PORT, "\x1b[0m");
})