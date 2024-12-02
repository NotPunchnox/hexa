const VARS = require('./src/vars/index.js');
const router = require('./src/router/index.js');

require('dotenv').config();

console.log('vars', VARS);

router.api();
router.websocket();