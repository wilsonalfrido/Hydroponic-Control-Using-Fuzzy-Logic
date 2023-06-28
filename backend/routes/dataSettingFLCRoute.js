const express = require('express');
const { saveDataSettingFLC,getDataSettingFLC,updateDataSettingFLC,getDataSettingFLCByStatus } = require('../controller/dataSettingFLCController.js');

const router = express.Router();

router.post('/dataSettingFLC',saveDataSettingFLC);
router.get('/dataSettingFLC/csv',getDataSettingFLC);
router.get('/dataSettingFLC/:status',getDataSettingFLCByStatus);
router.post('/dataSettingFLCUpdate/:id',updateDataSettingFLC);

module.exports = router;



