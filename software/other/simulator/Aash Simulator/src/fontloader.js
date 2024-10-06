import { FontLoader } from 'three/examples/jsm/loaders/FontLoader';
import { TextGeometry } from 'three/examples/jsm/geometries/TextGeometry';
import * as THREE from 'three';


// Chargement de la police et ajout des labels X, Y, Z
function LoadFont(scene) {
    const fontLoader = new FontLoader();
    fontLoader.load('https://threejs.org/examples/fonts/helvetiker_regular.typeface.json', (font) => {
      const createLabel = (text, position, color) => {
        const textGeometry = new TextGeometry(text, {
          font: font,
          size: 1,
          height: 0.1
        });
        const textMaterial = new THREE.MeshBasicMaterial({ color: color });
        const mesh = new THREE.Mesh(textGeometry, textMaterial);
        mesh.position.copy(position);
        return mesh;
      };
    
      // Ajout des labels pour chaque axe
      scene.add(createLabel('X', new THREE.Vector3(21, 0, 0), 0xff0000));
      scene.add(createLabel('Y', new THREE.Vector3(0, 21, 0), 0x00ff00));
      scene.add(createLabel('Z', new THREE.Vector3(0, 0, 21), 0x0000ff));
    });
}

export default LoadFont