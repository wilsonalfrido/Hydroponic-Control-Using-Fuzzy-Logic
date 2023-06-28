const { data_setting_flc,data_uji_flc } = require('../db/models/index.js');
const path = require("path");
const data_exporter = require("json2csv").Parser ;


const getUjiFLCbyIdTanam = async(req,res)=>{
    const id = req.params.id;
    try{
        var file_header = ['id','ph','ec','water_temp','water_level','ab_pump','water_pump','id_flc','createdAt'];
        const data = await data_uji_flc.findAll({
            where:{
                id_flc:id
            },attributes:['id','ph','ec','water_temp','water_level','ab_pump','water_pump','id_flc','createdAt']
        });
        if(data != null){
            var mysql_data = JSON.parse(JSON.stringify(data));
            var json_data = new data_exporter({file_header});
            var csv_data = json_data.parse(mysql_data);
    
            res.setHeader("Content-Type","text/csv");
            res.setHeader("Content-Disposition",`attachment;filename=DataUjiFLC_${id}.csv`);
            // res.send(csv_data);
            res.status(200).end(csv_data);

        } else {
            res.status(200).json({msg:`Data uji FLC dengan id ${id} tidak ditemukan`});
        }
    } catch(error) {
        res.status(400).json({msg:error.message});
    }
}

const getUjiFLCbyIdTanamJSON = async(req,res)=>{
    const id = req.params.id;
    try{
        const data = await data_uji_flc.findAll({
            where:{
                id_flc:id
            },attributes:['id','ph','ec','water_temp','water_level','ab_pump','water_pump','id_flc','createdAt']
        });
        if(data != null){
            res.status(200).json(data);
        } else {
            res.status(200).json({msg:`Data uji FLC dengan id ${id} tidak ditemukan`});
        }
    } catch(error) {
        res.status(400).json({msg:error.message});
    }

}


const saveUjiFLC = async(req,res)=>{
    const ph = req.body.ph;
    const ec = req.body.ec;
    const water_temp = req.body.water_temp;
    const water_level = req.body.water_level;
    const ab_pump = req.body.ab_pump;
    const water_pump = req.body.water_pump;

    const dataSettingFLCOnProgress = await data_setting_flc.findAll({
        // order: [[ 'id', 'ASC' ]],   
        where:{
            status:0
        }
    });

    try{
        if(dataSettingFLCOnProgress.length > 0){
            await data_uji_flc.create({
                ph:ph,
                ec:ec,
                water_temp:water_temp,
                water_level:water_level,
                ab_pump:ab_pump,
                water_pump:water_pump,
                id_flc:dataSettingFLCOnProgress[0].id
        });
            res.status(200).json({msg: "Data Berhasil Dimasukkan"});
        } else {
            res.status(200).json({msg: "Tidak ada data setting FLC yang sedang berjalan"});
        }
    } catch(error){
        console.log(error.message);
        res.status(400).json({msg:error.message});
    }
}

exports.saveUjiFLC = saveUjiFLC;
exports.getUjiFLCbyIdTanam = getUjiFLCbyIdTanam;
exports.getUjiFLCbyIdTanamJSON = getUjiFLCbyIdTanamJSON;