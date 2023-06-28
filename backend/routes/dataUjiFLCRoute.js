const express = require('express');
const { getUjiFLCbyIdTanam,getUjiFLCbyIdTanamJSON,saveUjiFLC } = require('../controller/dataUjiFLCController.js');

const router = express.Router();

router.post('/ujiFLC',saveUjiFLC);
router.get('/ujiFLC/csv/:id',getUjiFLCbyIdTanam);
router.get('/ujiFLC/json/:id',getUjiFLCbyIdTanamJSON);


module.exports = router;


