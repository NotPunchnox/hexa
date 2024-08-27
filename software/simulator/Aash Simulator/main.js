import './style.css';
import * as THREE from 'three';
import { xAxis, yAxis, zAxis } from './src/axis';
import EventsLoader from "./src/events.js";
import LoadFont from './src/fontloader.js';
import SetCamera from './src/Camera.js';
import CreateRobot from './src/Body.js';

const scene = new THREE.Scene();

const camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);

const renderer = new THREE.WebGLRenderer({
  canvas: document.querySelector('#bg')
});


renderer.setPixelRatio(window.devicePixelRatio);
renderer.setSize(window.innerWidth, window.innerHeight);
camera.position.setZ(30);

scene.add(xAxis, yAxis, zAxis);
EventsLoader(camera, renderer);
LoadFont(scene);
SetCamera(scene, camera, renderer);

CreateRobot(scene);