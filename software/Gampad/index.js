const { SerialPort, ReadlineParser } = require('serialport');

const portRemote = new SerialPort({ path: 'COM4', baudRate: 9600 });
const RobotPort = new SerialPort({ path: 'COM3', baudRate: 115200 });

const parserRemote = portRemote.pipe(new ReadlineParser({ delimiter: '\n' }));
const parserRobot = RobotPort.pipe(new ReadlineParser({ delimiter: '\n' }));

let lX2 = 0, lY2 = 0;
let lX = 0, lY = 0;
let Total = 0;

const MID_X = 512;
const MID_Y = 512;
const TOLERANCE = 30;
const vitesse = 1.5;

let command = '';
let commandTop = '';
let turning = false;

function calculateDirection(value, midValue) {
    const range = 1023; // Assuming joystick range is 0 to 1023
    const scaledValue = Math.floor(((value - midValue) / (range / 2)) * 3);
    return Math.max(-3, Math.min(3, scaledValue)); // Clamp values between -5 and 5
}

async function sendCommand(command) {
    return new Promise((resolve, reject) => {
        RobotPort.write(command + '\n', (err) => {
            if (err) {
                console.log('Error on write: ', err.message);
                return reject(err);
            }

             console.log('Command sent:', command);

        });
    });
}

parserRemote.on('data', async data => {
    const match = data.trim().match(/Joystick 2 - X = (\d{1,3}), Y = (\d{1,3})/);
    const match2 = data.trim().match(/Joystick 1 - X = (\d{1,3}), Y = (\d{1,3})/);

    if (match) {
        // const [xValue, yValue] = [parseInt(match[1]), parseInt(match[2])];

        // if (xValue === lX && yValue === lY) return;

        // lX = xValue;
        // lY = yValue;

        // const directionX = (Math.abs(xValue - MID_X) > TOLERANCE*2) ? calculateDirection(xValue, MID_X) : 0;
        // let directionY = (Math.abs(yValue - MID_Y) > TOLERANCE*2) ? calculateDirection(yValue, MID_Y) : 0;
        // if(Math.abs(yValue - 102) <= 3) directionY = 4
        // const newCommand = `ChangeXY_${vitesse}_${directionX}_${directionY}`;

        // if (command !== newCommand) {
        //     command = newCommand;

        //     try {
        //         await sendCommand(command);
        //         await setTimeout(() => {}, 2000);
        //     } catch (err) {
        //         console.error('Failed to send command:', err);
        //     }
        // }

        const [xValue, yValue] = [parseInt(match[1]), parseInt(match[2])];

        if (xValue === lX && yValue === lY) return;

        lX = xValue;
        lY = yValue;

        const directionX = (Math.abs(xValue - MID_X) > TOLERANCE*2) ? calculateDirection(xValue, MID_X) : 0;
        let directionY = (Math.abs(yValue - MID_Y) > TOLERANCE*2) ? calculateDirection(yValue, MID_Y) : 0;
        if(Math.abs(yValue - 102) <= 3) directionY = 3;
        let newCommand = `StartWalk_${vitesse}_${directionX*-1}_${directionY*-1}`;

        if(directionX == 0 && directionY == 0) newCommand = `StopWalk`;

        if (command !== newCommand) {
            command = newCommand;

            try {
                await sendCommand(command);
                // await setTimeout(() => {}, 1000);
            } catch (err) {
                console.error('Failed to send command:', err);
            }
        }
    } else if(match2) {
        const [xValue, yValue] = [parseInt(match2[1]), parseInt(match2[2])];

        if (xValue == lX2 && yValue == lY2) return;

        lX2 = xValue;
        lY2 = yValue;

        // console.log(yValue, Math.abs(yValue - MID_Y) > (TOLERANCE), calculateDirection(yValue, MID_Y))
        let directionY = (Math.abs(yValue - MID_Y) > (TOLERANCE)) ? calculateDirection(yValue, MID_Y) : 0;
        let directionX = (Math.abs(xValue - MID_X) > TOLERANCE*2) ? calculateDirection(xValue, MID_X) : 0;

        if(directionY !== 0 && directionX === 0){
            directionY > 0 ? directionY = -1 : directionY = 1;
            if (Total + directionY > 5 || Total + directionY < -5 ) return;
            Total = Total+directionY
            newCommand = `ChangeTop_${vitesse}_${directionY}`;
        } else if(directionX !== 0 && directionY === 0) {//StartTurn_1_left_1
            newCommand = `StartTurn_${vitesse/4}_${directionX > 0 ? "right" : "left"}_${Math.abs(directionX/4)}`
            turning = true;
        } else {
            newCommand = `StopTurn`;
        }
        
        if(commandTop === newCommand && !newCommand.includes('ChangeTop') || newCommand === 'StopTurn' && turning == false) return;
        if(newCommand === 'StopTurn') turning = false
        commandTop = newCommand;
        
        await sendCommand(newCommand);
    }
});
