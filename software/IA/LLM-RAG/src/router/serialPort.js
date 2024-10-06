import { SerialPort } from "serialport";

let port = "";

let list = await SerialPort.list();

let found = false;

list.forEach(a => {
    if (a.friendlyName.includes('CP210x')) {
        port = a.path;
        found = true;
    }
});

if (!found) {
    console.clear()
    throw new Error("\x1b[31mVeuillez Brancher le microcontrolleur du robot: Connexion au CP210x manquante !\x1b[0m");
}

export default port;
