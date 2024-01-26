const servoConfig = require('../vars/SERVO_CONFIG')

module.exports = (pose, leg, name) => {
    console.log(servoConfig[leg][name].min, pose[leg][name])

    if(servoConfig[leg][name].min >= pose[leg][name]) return false
    if(servoConfig[leg][name].max <= pose[leg][name]) return false
    
    return true

}