function EventsLoader(camera, renderer) {
    window.addEventListener('resize', () => {
        // Gérer le redimensionnement de la fenêtre
        camera.aspect = window.innerWidth / window.innerHeight;
        camera.updateProjectionMatrix();
        renderer.setSize(window.innerWidth, window.innerHeight);
    });
}

export default EventsLoader;