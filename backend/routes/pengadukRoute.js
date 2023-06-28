const express = require("express") ;

const { saveSCN, getSCN } = require("../controller/pengadukController.js");


const router = express.Router();

router.post('/api/scn',saveSCN);
router.get('/api/csv',getSCN) ;




module.exports = router;