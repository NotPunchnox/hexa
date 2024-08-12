const { SerialPort, ReadlineParser } = require('serialport');

const portRemote = new SerialPort({ path: 'COM7', baudRate: 9600 });
const RobotPort = new SerialPort({ path: 'COM4', baudRate: 115200 });

const parserRemote = portRemote.pipe(new ReadlineParser({ delimiter: '\n' }));
const parserRobot = RobotPort.pipe(new ReadlineParser({ delimiter: '\n' }));

let lX2 = 0, lY2 = 0;
let lX = 0, lY = 0;
const MID_X = 512;
const MID_Y = 512;
const TOLERANCE = 10;
const vitesse = 0.5;

let command = '';
let commandTop = '';

function calculateDirection(value, midValue) {
    const range = 1023; // Assuming joystick range is 0 to 1023
    const scaledValue = Math.floor(((value - midValue) / (range / 2)) * 5);
    return Math.max(-5, Math.min(5, scaledValue)); // Clamp values between -5 and 5
}

async function sendCommand(command) {
    return new Promise((resolve, reject) => {
        RobotPort.write(command + '\n', (err) => {
            if (err) {
                console.log('Error on write: ', err.message);
                return reject(err);
            }

            console.log('Command sent:', command);

            
            setTimeout(() => {
                parserRobot.once('data', (data) => {
                    const response = data.trim();
                    console.log("ROBOT:", response);
                    resolve(response);
                });
            }, 1000);
        });
    });
}

parserRemote.on('data', async data => {
    const match = data.trim().match(/Joystick 2 - X = (\d{1,3}), Y = (\d{1,3})/);
    const match2 = data.trim().match(/Joystick 1 - X = (\d{1,3}), Y = (\d{1,3})/);

    if (match) {
        const [xValue, yValue] = [parseInt(match[1]), parseInt(match[2])];

        if (xValue === lX && yValue === lY) return;

        lX = xValue;
        lY = yValue;

        const directionX = (Math.abs(xValue - MID_X) > TOLERANCE) ? calculateDirection(xValue, MID_X) : 0;
        const directionY = (Math.abs(yValue - MID_Y) > TOLERANCE) ? calculateDirection(yValue, MID_Y) : 0;

        const newCommand = `ChangeXY_${vitesse}_${directionX}_${directionY}`;

        if (command !== newCommand) {
            command = newCommand;

            try {
                await sendCommand(command);
            } catch (err) {
                console.error('Failed to send command:', err);
            }
        }
    } else if(match2) {
        const [xValue, yValue] = [parseInt(match2[1]), parseInt(match2[2])];

        if (xValue == lX2 && yValue == lY2) return;

        lX2 = xValue;
        lY2 = yValue;

        let directionY = (Math.abs(yValue - MID_Y) > TOLERANCE) ? calculateDirection(yValue, MID_Y) : 0;
        if(directionY > 0) {directionY = -1} else directionY = 1

        const newCommand = `ChangeTop_${vitesse}_${directionY}`;

            try {
                await sendCommand(newCommand);
            } catch (err) {
                console.error('Failed to send command:', err);
            }
    }
});
