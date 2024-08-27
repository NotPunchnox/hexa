import * as THREE from 'three';


function calculateInverseKinematics(x, y, z) {
    const coxaLength = 5; // Longueur de la Coxa
    const femurLength = 6.3; // Longueur du Fémur
    const tibiaLength = 13; // Longueur du Tibia

    const TPatte = Math.sqrt(x * x + y * y); // Taille de la patte vue du dessus
    const hyp = Math.sqrt(z * z + (TPatte - coxaLength) * (TPatte - coxaLength)); // Hypoténuse formée par Coxa et Tibia

    const A1 = Math.atan((TPatte - coxaLength) / z);
    const A2 = Math.acos((femurLength * femurLength + hyp * hyp - tibiaLength * tibiaLength) / (2 * femurLength * hyp));
    const angleFemur = THREE.MathUtils.radToDeg(A1 + A2);

    const A3 = Math.acos((tibiaLength * tibiaLength + femurLength * femurLength - hyp * hyp) / (2 * tibiaLength * femurLength));
    const angleTibia = THREE.MathUtils.radToDeg(A3);

    const angleCoxa = THREE.MathUtils.radToDeg(Math.atan2(y, x)) + 45;

    return { angleCoxa, angleFemur, angleTibia };
}

export default calculateInverseKinematics