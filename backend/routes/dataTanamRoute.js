const express = require('express');

const { saveDataTanam,getDataTanamCSV,getDataTanamByStatus,updateDataTanam } = require('../controller/dataTanamController.js');

const router = express.Router();

router.post('/dataTanam',saveDataTanam);
router.get('/dataTanam/csv',getDataTanamCSV);
router.get('/dataTanam/:status',getDataTanamByStatus);
router.post('/dataTanamUpdate/:id',updateDataTanam);

module.exports = router;