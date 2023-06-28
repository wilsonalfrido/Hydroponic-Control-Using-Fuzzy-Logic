const express = require('express');

const { getLoraData, getLoraDataByHari,saveLoraData } = require('../controller/dataLoraController.js');

const router = express.Router();

router.get('/lora',getLoraData);
router.get('/lora/:hariKe',getLoraDataByHari);
router.post('/lora',saveLoraData);

module.exports = router;