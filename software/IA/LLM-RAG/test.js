import { SerialPort } from 'serialport'

const serialport = new SerialPort({ path: 'COM4', baudRate: 115200 })
serialport.write('ChangeTop_3_4')