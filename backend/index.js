const express  = require('express');
const cors = require("cors");
const FileUpload = require("express-fileupload");
const pengadukRoute = require("./routes/pengadukRoute.js");
const dataLoraRoute = require("./routes/dataLoraRoute.js");
const dataTanamRoute = require("./routes/dataTanamRoute.js");
const dataSCNRoute = require("./routes/dataSCNRoute.js");
const data_setting_flc = require("./routes/dataSettingFLCRoute.js");
const data_uji_flc = require("./routes/dataUjiFLCRoute.js");
const data_history_pompa_flc = require("./routes/dataHistoryPompaRoute.js");

const app = express();
app.use(cors());
app.use(express.json());
app.use(FileUpload());
app.use(pengadukRoute);
app.use(dataLoraRoute);
app.use(dataTanamRoute);
app.use(dataSCNRoute);
app.use(data_setting_flc);
app.use(data_uji_flc);
app.use(data_history_pompa_flc);
app.use(express.static("public"));


app.listen(5000,()=> console.log('Server running at port 5000'));