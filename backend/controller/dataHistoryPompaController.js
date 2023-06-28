const {data_history_pompa,dataTanam} = require('../db/models/index.js');
const path = require("path");
const data_exporter = require("json2csv").Parser ;


const getDataHistoryPompa = async(req,res)=>{
    try{
        var file_header = ['id','offset_ec','ec','ph','water_level','water_temp','ab_dur','wp_dur','set_water_level','set_ec','hariKe','id_dataTanam','flc_mode','createdAt'];
        const data = await data_history_pompa.findAll(
            {
                attributes:['id','offset_ec','ec','ph','water_level','water_temp','ab_dur','wp_dur','set_water_level','set_ec','hariKe','id_dataTanam','flc_mode','createdAt']
            }
        );
        var mysql_data = JSON.parse(JSON.stringify(data));
        var json_data = new data_exporter({file_header});
        var csv_data = json_data.parse(mysql_data);

        res.setHeader("Content-Type","text/csv");
        res.setHeader("Content-Disposition","attachment;filename=dataHistoryPompa.csv");
        // res.send(csv_data);
        res.status(200).end(csv_data);

    } catch(error) {
        res.status(400).json({msg:error.message});
    }
}

const getDataHistoryPompaJson = async(req,res)=>{
    try{
        const data = await data_history_pompa.findAll(
            {
                attributes:['id','offset_ec','ec','ph','water_level','water_temp','ab_dur','wp_dur','set_water_level','set_ec','hariKe','id_dataTanam','flc_mode','createdAt']
            }
        );
        if(data != null){
            res.status(200).json(data);
        }else{
            res.status(400).json({msg:"Data not found"});
        }
    } catch(error) {
        res.status(400).json({msg:error.message});
    }
}

const getDataHistoryPompaById = async(req,res)=>{
    try{
        const id = req.params.id;
        const data = await data_history_pompa.findAll({
            where:{
                id_dataTanam:id
            },attributes:['id','offset_ec','ec','ph','water_level','water_temp','ab_dur','wp_dur','set_water_level','set_ec','hariKe','id_dataTanam','flc_mode','createdAt']
        });
        if(data != null){
            var file_header = ['id','offset_ec','ec','ph','water_level','water_temp','ab_dur','wp_dur','set_water_level','set_ec','hariKe','id_dataTanam','flc_mode','createdAt'];
            var mysql_data = JSON.parse(JSON.stringify(data));
            var json_data = new data_exporter({file_header});
            var csv_data = json_data.parse(mysql_data);

            res.setHeader("Content-Type","text/csv");
            res.setHeader("Content-Disposition","attachment;filename=dataHistoryPompa.csv");
            // res.send(csv_data);
            res.status(200).end(csv_data);
        }else{
            res.status(400).json({msg:"Data not found"});
        }
    } catch(error) {
        res.status(400).json({msg:error.message});
    }
}

const getDataHistoryPompaByIdJson = async(req,res)=>{
    try{
        const id = req.params.id;
        const data = await data_history_pompa.findAll({
            where:{
                id_dataTanam:id
            },attributes:['id','offset_ec','ec','ph','water_level','water_temp','ab_dur','wp_dur','set_water_level','set_ec','hariKe','id_dataTanam','flc_mode','createdAt']
        });
        if(data != null){
            res.status(200).json(data);
        }else{
            res.status(400).json({msg:"Data not found"});
        }
    } catch(error) {
        res.status(400).json({msg:error.message});
    }
}



const saveDataHistoryPompa = async(req,res)=>{
    const offset_ec = req.body.offset_ec;
    const ec = req.body.ec;
    const ph = req.body.ph;
    const water_level = req.body.water_level;
    const water_temp = req.body.water_temp;
    const ab_dur = req.body.ab_dur;
    const wp_dur = req.body.wp_dur;
    const set_water_level = req.body.set_water_level;
    const set_ec = req.body.set_ec;
    const flc_mode = req.body.flc_mode;

    const dataTanamOnProgress = await dataTanam.findAll({
        where:{
            status:0
    }});


    try{
        if(dataTanamOnProgress.length > 0){
            var date1 = new Date(dataTanamOnProgress[0].tanggal_tanam);
            var date2 = new Date();
        
            var Difference_In_Time = date2.getTime() - date1.getTime();
          
            // hitung jml hari antara dua tanggal
            const Difference_In_Days = Math.floor(Difference_In_Time / (1000 * 3600 * 24));
        const data = await data_history_pompa.create({
            offset_ec:offset_ec,
            ec:ec,
            ph:ph,
            water_level:water_level,
            water_temp:water_temp,
            ab_dur:ab_dur,
            wp_dur:wp_dur,
            set_water_level:set_water_level,
            set_ec:set_ec,
            hariKe:Difference_In_Days,
            id_dataTanam:dataTanamOnProgress[0].id,
            flc_mode:flc_mode
        });
        res.status(200).json({msg:"Data setting FLC berhasil disimpan"});
        } else {
            res.status(200).json({msg:"Data setting FLC masih ada yang belum selesai"});
        }
    } catch(error) {
        res.status(400).json({msg:error.message});
    }
}

const getDataHariKe = async(req,res)=>{
    const dataTanamOnProgress = await dataTanam.findAll({
        where:{
            status:0
    }});

    try{
        if(dataTanamOnProgress.length > 0){
            // var date1 = new Date();
            var date1 = new Date(dataTanamOnProgress[0].tanggal_tanam);
            var date2 = new Date();
        
            var Difference_In_Time = date2.getTime() - date1.getTime();
          
            // hitung jml hari antara dua tanggal
            const Difference_In_Days = Math.floor(Difference_In_Time / (1000 * 3600 * 24));
            res.status(200).json({hariKe:Difference_In_Days});
        } else {
            res.status(200).json({msg:"Tidak ada data penanaman yang sedang berlangsung"});
        }
        
    } catch(error) {
        res.status(400).json({msg:error.message});
    }

}

exports.saveDataHistoryPompa= saveDataHistoryPompa;
exports.getDataHistoryPompa= getDataHistoryPompa;
exports.getDataHistoryPompaById= getDataHistoryPompaById;
exports.getDataHistoryPompaJson= getDataHistoryPompaJson;
exports.getDataHistoryPompaByIdJson= getDataHistoryPompaByIdJson;
exports.getDataHariKe= getDataHariKe;
