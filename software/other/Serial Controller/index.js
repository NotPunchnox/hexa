const { SerialPort, ReadlineParser } = require('serialport');
const rd = require('readline-sync');

const Robot = new SerialPort({ path: 'COM3', baudRate: 115200 });

const parser = Robot.pipe(new ReadlineParser({ delimiter: '\n' }))

console.clear();
console.log(Robot)

Robot.write('TurnZ_1_horaire');
setTimeout(() => {}, 1000)
Robot.write('Rouli_2_0_0_4_-4');
setTimeout(() => {}, 1000)
Robot.write('ChangeY_2_5')
setTimeout(() => {}, 1000)
Robot.write('ChangeY_2_-5')

/*
while(true) {

    let question = rd.question('Commande: ');

    Robot.write(question)

        //+ '\n', err => {

        //if(err) return console.log('Erreur:', err);
        //console.log('Command sent:', command);

    //})
}*/
