import * as THREE from 'three';
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls';

function SetCamera(scene, camera, renderer) {
    // Ajout des lumières
    const pointLight = new THREE.PointLight(0xffffff);
    pointLight.position.set(20, 20, 20);

    const ambientLight = new THREE.AmbientLight(0xffffff);

    scene.add(pointLight, ambientLight);

    // Contrôle de la caméra
    const controls = new OrbitControls(camera, renderer.domElement);
    controls.enableDamping = true;

    function animate() {
        requestAnimationFrame(animate);

        controls.update();

        renderer.render(scene, camera);
    }


    animate();
}

export default SetCamera