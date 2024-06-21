/* Vision */
const fs = require('fs');
const Modal = require('./Modal.json');

module.exports = (array) => {
    array = JSON.stringify(array)

    if (Modal.history[Modal.history.length - 1].role === "model") {
        Modal.history.push({
            "role": "user",
            "parts": [{
                "text": `CAMERA: ${array}`
            }]
        })
    } else if(Modal.history[Modal.history.length-1].role === "user") {
        const PART = Modal.history[Modal.history.length-1].parts
        if(PART[PART.length-1].text === `CAMERA: ${array}`) return console.log('Last = identique');

        Modal.history[Modal.history.length-1].parts.push({
            text: `CAMERA: ${array}`
        })
    }

    const history = Modal.history

    console.log(Modal.history)
    fs.writeFileSync(__dirname + '/Modal_example.json', JSON.stringify({ history }, null, 2));
}