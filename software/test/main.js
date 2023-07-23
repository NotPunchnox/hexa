const { Board, Servo } = require("johnny-five");
const Raspi = require("raspi-io").RaspiIO;
const WebSocketServer = require('ws');
const { servoConfig, initServo, Conditions } = require('./src');

const wss = new WebSocketServer.Server({ port: require('./server.json').port });

const board = new Board({
  io: new Raspi()
});

board.on("ready", () => {
  console.log("Connected to Hexapod.");

  let hexapodServos = {};
  const LEG_POSITIONS = Object.getOwnPropertyNames(servoConfig);

  for (let leg of LEG_POSITIONS) {
    hexapodServos[leg] = {
      alpha: initServo(leg, "alpha", Servo),
      beta: initServo(leg, "beta", Servo),
      gamma: initServo(leg, "gamma", Servo),
    };
  }

  const setServo = (pose, leg, name) => {
    console.log(pose[leg])
    const newPose = pose[leg][name];
    const res = Conditions(pose, leg, name)
    console.log(res)
    if(!res) return;

    hexapodServos[leg][name].to(newPose);
    console.log('success')
  };

  const setHexapodPose = pose => {
    for (let leg of LEG_POSITIONS) {
      setServo(pose, leg, "alpha");
      setServo(pose, leg, "beta");
      setServo(pose, leg, "gamma");
    }
  };

  wss.on("connection", ws => {
    console.log("Client connected.");

    ws.on("message", data => {
      const msg = JSON.parse(data);

      if (msg.pose) {
        setHexapodPose(msg.pose);
      }
    });

    ws.on("close", () => {
      console.log("Client disconnected.");
    });
  });

  console.log('WebSocket server listening on port 4000');
});