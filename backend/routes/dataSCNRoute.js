const express = require('express');

const { saveSCN,getSCNbyIdTanam} = require('../controller/dataSCNController.js');

const router = express.Router();

router.post('/scn',saveSCN);
router.get('/scn/csv/:id',getSCNbyIdTanam);


module.exports = router;