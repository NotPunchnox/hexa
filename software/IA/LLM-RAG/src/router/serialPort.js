import { SerialPort } from "serialport";
import config from "../../config.json" assert { type: "json" }

let port = "";

let list = await SerialPort.list();

let found = false;

list.forEach(a => {
    if (a.friendlyName.includes('CP210x')) {
        port = a.path;
        found = true;
    }
});

if (!found && config.serial) {
    console.clear()
    throw new Error("\x1b[31mVeuillez Brancher le microcontrolleur du robot: Connexion au CP210x manquante !\x1b[0m");
}

export default port;
