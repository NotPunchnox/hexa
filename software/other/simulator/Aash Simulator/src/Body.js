import * as THREE from 'three';
import calculateInverseKinematics from './InverseKinematics';

function CreateRobot(scene) {
    // Création du corps du robot
    const geometry = new THREE.BoxGeometry(12, 1, 8);
    const material = new THREE.MeshBasicMaterial({ color: 0xc0f2e7 });
    const body = new THREE.Mesh(geometry, material);
    body.position.set(0, 0.5, 0);
    scene.add(body);

    // Fonction pour créer une patte avec des cylindres et des sphères pour les articulations
    function createLeg() {
        const legGroup = new THREE.Group();

        // Matériau pour les segments
        const segmentMaterial = new THREE.MeshBasicMaterial({ color: 0x808080 });
        // Matériau pour les articulations
        const jointMaterial = new THREE.MeshBasicMaterial({ color: 0xff0000 });

        // Coxa (le segment attaché au corps)
        const coxaGeometry = new THREE.CylinderGeometry(0.1, 0.1, 3, 32);
        coxaGeometry.translate(0, 0, 1.5); // Déplacer la géométrie pour que le pivot soit à une extrémité
        const coxa = new THREE.Mesh(coxaGeometry, segmentMaterial);
        legGroup.add(coxa);

        // Fémur (segment intermédiaire)
        const femurGeometry = new THREE.CylinderGeometry(0.1, 0.1, 4, 32);
        femurGeometry.translate(0, 0, 2); // Déplacer la géométrie pour que le pivot soit à une extrémité
        const femur = new THREE.Mesh(femurGeometry, segmentMaterial);
        femur.position.set(0, 0, 3); // Position par rapport à Coxa
        coxa.add(femur); // Ajouter le fémur à la coxa

        // Tibia (segment final)
        const tibiaGeometry = new THREE.CylinderGeometry(0.1, 0.1, 5, 32);
        tibiaGeometry.translate(0, 0, 2.5); // Déplacer la géométrie pour que le pivot soit à une extrémité
        const tibia = new THREE.Mesh(tibiaGeometry, segmentMaterial);
        tibia.position.set(0, 0, 4); // Position par rapport au fémur
        femur.add(tibia); // Ajouter le tibia au fémur

        // Ajouter les sphères pour représenter les articulations
        const coxaJoint = new THREE.Mesh(new THREE.SphereGeometry(0.2, 32, 32), jointMaterial);
        coxaJoint.position.set(0, 0, 0);
        legGroup.add(coxaJoint);

        const femurJoint = new THREE.Mesh(new THREE.SphereGeometry(0.2, 32, 32), jointMaterial);
        femurJoint.position.set(0, 0, 3);
        coxa.add(femurJoint);

        const tibiaJoint = new THREE.Mesh(new THREE.SphereGeometry(0.2, 32, 32), jointMaterial);
        tibiaJoint.position.set(0, 0, 4);
        femur.add(tibiaJoint);

        return legGroup;
    }

    // Ajouter la patte au corps
    const leg = createLeg();
    leg.position.set(-6, 0.5, 4);
    body.add(leg);

    // Fonction pour appliquer la cinématique inverse
    function applyInverseKinematics() {
        const { angleCoxa, angleFemur, angleTibia } = calculateInverseKinematics(12, 0, 8);
        const coxa = leg.children[1];
        const femur = coxa.children[0];
        const tibia = femur.children[0];

        coxa.rotation.z = THREE.MathUtils.degToRad(angleCoxa);
        femur.rotation.z = THREE.MathUtils.degToRad(angleFemur);
        tibia.rotation.z = THREE.MathUtils.degToRad(angleTibia);

        console.log({ angleCoxa, angleFemur, angleTibia });
    }

    // Initialiser les rotations
    const coxa = leg.children[0];
    const femur = coxa.children[0];
    const tibia = femur.children[0];

    coxa.rotation.z = THREE.MathUtils.degToRad(45);

    // Appliquer la cinématique inverse après un délai
    // setTimeout(() => {
    //     applyInverseKinematics();
    // }, 1000);
}

export default CreateRobot;
