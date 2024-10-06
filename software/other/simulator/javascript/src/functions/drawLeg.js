module.exports = (plot, startPoint, angles, lengths, mode) => {
    const [x, y, z] = startPoint;

    // Calcul des coordonnées finales de chaque segment en fonction des angles et des longueurs
    const endX = x + lengths[0] * Math.cos(angles[0]);
    const endY = y + lengths[0] * Math.sin(angles[0]);
    const endZ = z;

    const midX = endX + lengths[1] * Math.cos(angles[1]);
    const midY = endY + lengths[1] * Math.sin(angles[1]);
    const midZ = endZ;

    const finalX = midX + lengths[2] * Math.cos(angles[2]);
    const finalY = midY + lengths[2] * Math.sin(angles[2]);
    const finalZ = midZ;

    // Dessin des segments de la patte
    plot([{
            x: [x, endX],
            z: [y, endY],
            y: [z, endZ],
            type: 'scatter3d',
            name: 'Coxa',
            mode,
            line: { color: 'green' }
        },
        {
            x: [endX, midX],
            z: [endY, midY],
            y: [endZ, midZ],
            type: 'scatter3d',
            mode,
            name: 'Femur',
            line: { color: 'blue' }
        },
        {
            x: [midX, finalX],
            z: [midY, finalY],
            y: [midZ, finalZ],
            type: 'scatter3d',
            mode,
            name: 'Tibia',
            line: { color: 'red' }
        }
    ], {
        scene: {
            xaxis: {
                title: 'X'
            },
            yaxis: {
                title: 'Z'
            }, // Mettre 'Z' pour l'axe y
            zaxis: {
                title: 'Y'
            }, // Mettre 'Y' pour l'axe z
        }
    });
}