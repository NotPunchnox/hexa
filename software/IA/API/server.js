const express = require('express');
const app = express();
const run = require('./src/loadQuestion');
const PORT = 3000;

app.use(express.json());


app.get('/api/question', async(req, res) => {
  if(!req.body.content) return res.status(400).json({code: 400, message: "Bad request: no content"});
  console.log("Nouvelle question:", req.body.content)
  var text = await run(req.body.content);

  try {
    text = JSON.parse(text);
  } catch(err) {
    console.error(err);
  }

  res.status(201).json({code: 201, content: text});
})

app.listen(PORT, () => {
  console.log("\x1b[32mLe serveur est en route sur le port: ", PORT, "\x1b[0m");
})