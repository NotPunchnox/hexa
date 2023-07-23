// See configurable parameters at: http://johnny-five.io/api/servo/

const controller = "PCA9685"
const address = null//"0x41"

module.exports = {
    leftFront: {
        alpha: { controller, pin: 0, min: 2, max: 180, address },
        beta: { controller, pin: 1, min: 2, max: 130, address },
        gamma: { controller, pin: 2, min: 2, max: 170, address },
    },

    /*
    rightFront: {
        alpha: { controller, pin: 15, address },
        beta: { pin: 3 },
        gamma: { pin: 5 },
    },

    rightMiddle: {
        alpha: { controller, pin: 6, address },
        beta: { controller, pin: 8, address },
        gamma: { controller, pin: 7, address },
    },

    leftMiddle: {
        alpha: { controller, pin: 10, address },
        beta: { controller, pin: 9, address },
        gamma: { controller, pin: 11, address },
    },

    leftBack: {
        alpha: { controller, pin: 2, address },
        beta: { controller, pin: 1, address },
        gamma: { controller, pin: 0, address },
    },

    rightBack: {
        alpha: { controller, pin: 5, address },
        beta: { controller, pin: 4, address },
        gamma: { controller, pin: 3, address },
    }
    */

}