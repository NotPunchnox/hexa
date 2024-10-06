import * as THREE from 'three';


// Création des axes personnalisés
const createAxis = (points, color) => {
    const geometry = new THREE.BufferGeometry().setFromPoints(points);
    const material = new THREE.LineBasicMaterial({ color: color });
    return new THREE.Line(geometry, material);
  };
  
  // Axe X (rouge)
  const xPoints = [new THREE.Vector3(0, 0, 0), new THREE.Vector3(20, 0, 0)];
  const xAxis = createAxis(xPoints, 0xff0000);
  
  // Axe Y (vert)
  const yPoints = [new THREE.Vector3(0, 0, 0), new THREE.Vector3(0, 20, 0)];
  const yAxis = createAxis(yPoints, 0x00ff00);
  
  // Axe Z (bleu)
  const zPoints = [new THREE.Vector3(0, 0, 0), new THREE.Vector3(0, 0, 20)];
  const zAxis = createAxis(zPoints, 0x0000ff);

export {
    xAxis,
    yAxis,
    zAxis
}