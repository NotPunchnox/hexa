const {plot} = require('nodeplotlib');
const Hexa = require('./src/')

Hexa.router.axes(plot)

const config = {
    coxa: 5,
    femur: 6,
    tibia: 14
}
const coordinated = {
    x: 12,
    y: 8,
    z: 0
}
Hexa.Algorithmes.inverseKinematic({config, coordinated})


// Définir les paramètres de la patte (exemples)
const startPoint = [5, 10, 10];
const angles = [0, 70, (30)];
const lengths = [5, 6, 14];
const mode = "lines"

// Dessiner la patte
Hexa.Controllers.DrawLeg(plot, startPoint, angles, lengths, mode);