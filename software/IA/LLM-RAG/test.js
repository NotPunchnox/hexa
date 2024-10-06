import {SerialPort} from "serialport";

let port = "";

let list = await SerialPort.list()

list.forEach(a => {
    if(a.friendlyName.includes('CP210x')) return port = a.path
})

console.log("Port:", port)