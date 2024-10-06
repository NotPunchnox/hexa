const x = [0, 1, 2, 3, 4];
const y = [0, 1, 2, 3, 4];
const z = [0, 1, 2, 3, 4];


const data = [{
    x: x,
    y: z,
    z: y,
    type: 'scatter3d'
}];

// Configurer les options de la scène
const layout = {
    scene: {
        xaxis: { title: 'X' },
        yaxis: { title: 'Z' },
        zaxis: { title: 'Y' },
    }
};

module.exports = plot => plot(data, layout);