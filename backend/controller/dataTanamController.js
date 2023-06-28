const { dataTanam } = require("../db/models/index.js");
const path = require("path");
const data_exporter = require("json2csv").Parser ;

const getDataTanamCSV = async(req,res)=>{
    try{
        var file_header = ['id','tanggal_semai','tanggal_tanam','tanggal_panen','status'];
        const data = await dataTanam.findAll(
            {
                attributes:['id','tanggal_semai','tanggal_tanam','tanggal_panen','status']
            }
        );
        var mysql_data = JSON.parse(JSON.stringify(data));
        var json_data = new data_exporter({file_header});
        var csv_data = json_data.parse(mysql_data);

        res.setHeader("Content-Type","text/csv");
        res.setHeader("Content-Disposition","attachment;filename=dataTanam.csv");
        // res.send(csv_data);
        res.status(200).end(csv_data);

    } catch(error) {
        res.status(400).json({msg:error.message});
    }
}

const getDataTanamByStatus = async(req,res)=>{
    try {
        const data = await dataTanam.findAll({
            where:{
                status:req.params.status
            },attributes:['id','tanggal_semai','tanggal_tanam','tanggal_panen','status']
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

const saveDataTanam = async(req,res)=>{
    const tanggalTanam = req.body.tanggalTanam;
    const tanggalSemai = req.body.tanggalSemai;
    
    const dataTanamOnProgress = await dataTanam.findAll({
        where:{
            status:0
    }});

    try{
        if(dataTanamOnProgress.length <= 0){
        const data = await dataTanam.create({
            tanggal_semai: tanggalSemai,
            tanggal_tanam: tanggalTanam,
            status: 0
        });
        res.status(200).json({msg:"Data tanam berhasil disimpan"});
        } else {
            res.status(200).json({msg:"Data tanam masih ada yang belum selesai"});
        }
    } catch(error) {
        res.status(400).json({msg:error.message});
    }
}

const updateDataTanam = async(req,res)=>{
    const dateNow = new Date();
    
    try{
        const data = await dataTanam.update({
            tanggal_panen: dateNow.toISOString(),
            status: 1
        },{
            where:{
                id:req.params.id
            }
        });
        res.status(200).json({msg:"Data tanam berhasil diupdate"});
    } catch(error) {
        res.status(400).json({msg:error.message});
    }
}

exports.getDataTanamCSV = getDataTanamCSV;
exports.saveDataTanam = saveDataTanam;
exports.getDataTanamByStatus = getDataTanamByStatus;
exports.updateDataTanam = updateDataTanam;


