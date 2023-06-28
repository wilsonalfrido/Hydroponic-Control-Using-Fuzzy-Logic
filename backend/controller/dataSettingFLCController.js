const { data_setting_flc } = require("../db/models/index.js");
const path = require("path");
const data_exporter = require("json2csv").Parser ;

const getDataSettingFLC = async(req,res)=>{
    try{
        var file_header = ['id','offset_ec','ec','ph','water_level','water_temp','ab_dur','wp_dur','set_water_level','set_ec','status'];
        const data = await data_setting_flc.findAll(
            {
                attributes:['id','offset_ec','ec','ph','water_level','water_temp','ab_dur','wp_dur','set_water_level','set_ec','status']
            }
        );
        var mysql_data = JSON.parse(JSON.stringify(data));
        var json_data = new data_exporter({file_header});
        var csv_data = json_data.parse(mysql_data);

        res.setHeader("Content-Type","text/csv");
        res.setHeader("Content-Disposition","attachment;filename=dataSettingFLC.csv");
        // res.send(csv_data);
        res.status(200).end(csv_data);

    } catch(error) {
        res.status(400).json({msg:error.message});
    }
}

const getDataSettingFLCByStatus = async(req,res)=>{
    try {
        const data = await data_setting_flc.findAll({
            where:{
                status:req.params.status
            },attributes: ['id','offset_ec','ec','ph','water_level','water_temp','ab_dur','wp_dur','set_water_level','set_ec','status']
        });
        if(data != null){
            res.status(200).json(data);
        } else {
            res.status(200).json({msg:`Data dengan status ${req.params.status} tidak ditemukan`});
        }
    } catch(error) {
        res.status(400).json({msg:error.message});
    }
}

const saveDataSettingFLC = async(req,res)=>{
    const offset_ec = req.body.offset_ec;
    const ec = req.body.ec;
    const ph = req.body.ph;
    const water_level = req.body.water_level;
    const water_temp = req.body.water_temp;
    const ab_dur = req.body.ab_dur;
    const wp_dur = req.body.wp_dur;
    const set_water_level = req.body.set_water_level;
    const set_ec = req.body.set_ec;

    const dataFLCOnProgress = await data_setting_flc.findAll({
        where:{
            status:0
    }});

    try{
        if(dataFLCOnProgress.length <= 0){
        const data = await data_setting_flc.create({
            offset_ec:offset_ec,
            ec:ec,
            ph:ph,
            water_level:water_level,
            water_temp:water_temp,
            ab_dur:ab_dur,
            wp_dur:wp_dur,
            set_water_level:set_water_level,
            set_ec:set_ec,
            status:0
        });
        res.status(200).json({msg:"Data setting FLC berhasil disimpan"});
        } else {
            res.status(200).json({msg:"Data setting FLC masih ada yang belum selesai"});
        }
    } catch(error) {
        res.status(400).json({msg:error.message});
    }
}

const updateDataSettingFLC = async(req,res)=>{    
    try{
        const data = await data_setting_flc.update({
            status: 1
        },{
            where:{
                id:req.params.id
            }
        });
        res.status(200).json({msg:"Data setting FLC berhasil diupdate"});
    } catch(error) {
        res.status(400).json({msg:error.message});
    }
}

exports.saveDataSettingFLC = saveDataSettingFLC;
exports.updateDataSettingFLC = updateDataSettingFLC;
exports.getDataSettingFLC = getDataSettingFLC;
exports.getDataSettingFLCByStatus = getDataSettingFLCByStatus;


