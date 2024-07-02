const fs = require('fs');
const history = require('./models/model.json');

let dialogues = [];
let currentInputs = [];

// Loop through each element in the history array
history.forEach((entry, index) => {
    if (entry.role === 'user') {
        currentInputs.push(entry.content);

        // If the next entry is "Hexa" or we are at the end of the array, add the dialogue
        if (history[index + 1] && history[index + 1].role === 'Hexa') {
            dialogues.push(String(JSON.stringify({
                input: currentInputs.length > 1 ? currentInputs : currentInputs[0],
                output: history[index + 1].content
            })));
            currentInputs = [];
        } else if (!history[index + 1]) {
            // If we are at the end of the array and the last entries are users
            dialogues.push(String(JSON.stringify({ input: currentInputs })));
            currentInputs = [];
        }
    } else if (entry.role === 'Hexa' && currentInputs.length > 0) {
        dialogues.push(String(JSON.stringify({
            input: currentInputs.length > 1 ? currentInputs : currentInputs[0],
            output: entry.content
        })));
        currentInputs = [];
    }
});

// Write the array of dialogues to the file model-format.json as a single JSON array
fs.writeFileSync('./model-format.json', JSON.stringify(dialogues, null, 2));

console.log('Les dialogues ont été regroupés et sauvegardés dans model-format.json.');
