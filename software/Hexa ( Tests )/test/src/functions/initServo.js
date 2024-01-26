const servoConfig = require('../vars/SERVO_CONFIG')

module.exports = (legPosition, angleName, Servo) => {
    const config = servoConfig[legPosition][angleName];
    const servo = new Servo({
      controller: "PCA9685",
      pin: config.pin,
      startAt: config.startAt,
    });
    console.log("LegPosition:", legPosition, "AngleName:", angleName);

    return servo;
}