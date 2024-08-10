const express = require('express');
const { SerialPort, ReadlineParser } = require('serialport');
const bodyParser = require('body-parser');
const cors = require('cors');

const app = express();
const port = new SerialPort({ path: 'COM4', baudRate: 115200 });

app.use(bodyParser.json());
app.use(cors())

app.post('/send-command', (req, res) => {
    const command = req.body.command;

    parser = port.pipe(new ReadlineParser({
        delimiter: '\n'
    }));

    port.write(command + '\n', function(err) {
        if (err) {
            console.log('Error on write: ', err.message);
            return res.status(500).send('Failed to send command');
        }
        console.log('Command sent: ', command);

        parser.once('data', (data) => {
            console.log("Info Hardware:", data.trim());
        });
        
        res.status(200).send('Command sent successfully');
    });
});

app.listen(3005, () => {
    console.log('Server listening on http://localhost:3005');
});
