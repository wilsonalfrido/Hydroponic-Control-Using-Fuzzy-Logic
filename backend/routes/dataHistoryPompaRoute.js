const express = require('express');

const { saveDataHistoryPompa,getDataHariKe,getDataHistoryPompa,getDataHistoryPompaById,getDataHistoryPompaJson,getDataHistoryPompaByIdJson } = require('../controller/dataHistoryPompaController.js');

const router = express.Router();

router.get('/dataHistoryPompa',getDataHistoryPompa);
router.get('/dataHistoryPompa/:id',getDataHistoryPompaById);
router.get('/dataHistoryPompaJson',getDataHistoryPompaJson);
router.get('/dataHistoryPompaJson/:id',getDataHistoryPompaByIdJson);
router.post('/dataHistoryPompa',saveDataHistoryPompa);
router.get('/dataHariKe',getDataHariKe);

module.exports = router;


