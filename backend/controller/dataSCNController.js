const { dataSCN,dataTanam } = require('../db/models/index.js');
const path = require("path");
const data_exporter = require("json2csv").Parser ;


const getSCNbyIdTanam = async(req,res)=>{
    const id = req.params.id;
    try{
        var file_header = ['id','ph','ec','water_temp','water_level','status','hariKe','id_dataTanam','ab_pump','water_pump','createdAt'];
        const data = await dataSCN.findAll({
            where:{
                id_dataTanam:id
            },attributes:['id','ph','ec','water_temp','water_level','status','hariKe','id_dataTanam','ab_pump','water_pump','createdAt']
        });
        if(data != null){
            var mysql_data = JSON.parse(JSON.stringify(data));
            var json_data = new data_exporter({file_header});
            var csv_data = json_data.parse(mysql_data);
    
            res.setHeader("Content-Type","text/csv");
            res.setHeader("Content-Disposition",`attachment;filename=DataSCN_${id}.csv`);
            // res.send(csv_data);
            res.status(200).end(csv_data);

        } else {
            res.status(200).json({msg:`Data dengan id ${id} tidak ditemukan`});
        }
    } catch(error) {
        res.status(400).json({msg:error.message});
    }
}

const getSCNbyWaterPumpOn = async(req,res)=>{
    // try{
    //     var file_header = ['id','ph','ec','water_temp','water_level','status','hariKe','id_dataTanam','ab_pump','water_pump','createdAt'];
    //     const data = await dataSCN.findAll({
    //         where:{
    //             waterPump:1
    //         },attributes:['id','ph','ec','water_temp','water_level','status','hariKe','id_dataTanam','ab_pump','water_pump','createdAt']
    //     });
    //     if(data != null){
    //         var mysql_data = JSON.parse(JSON.stringify(data));
    //         var json_data = new data_exporter({file_header});
    //         var csv_data = json_data.parse(mysql_data);
    
    //         res.setHeader("Content-Type","text/csv");
    //         res.setHeader("Content-Disposition",'attachment;filename=DataPompa.csv');
    //         // res.send(csv_data);
    //         res.status(200).end(csv_data);

    //     } else {
    //         res.status(200).json({msg:'Data tidak ditemukan'});
    //     }
    // } catch(error) {
    //     res.status(400).json({msg:error.message});
    // }
}




const saveSCN = async(req,res)=>{
    const ph = req.body.ph;
    const ec = req.body.ec;
    const water_temp = req.body.water_temp;
    const water_level = req.body.water_level;
    const water_pump = req.body.water_pump;
    const ab_pump = req.body.ab_pump;
    


    const dataTanam0 = await dataTanam.findAll({
        // order: [[ 'id', 'ASC' ]],   
        where:{
            status:0
        }
    });
 
    try{
        if(dataTanam0.length >0){
            var date1 = new Date(dataTanam0[0].tanggal_tanam);
            var date2 = new Date();
        
            var Difference_In_Time = date2.getTime() - date1.getTime();
          
            // hitung jml hari antara dua tanggal
            const Difference_In_Days = Math.floor(Difference_In_Time / (1000 * 3600 * 24));
            await dataSCN.create({ph:ph,ec:ec,water_temp:water_temp,water_level:water_level,id_dataTanam:dataTanam0[0].id,status:dataTanam0[0].status,hariKe:Difference_In_Days,ab_pump:ab_pump,water_pump:water_pump});
            res.status(201).json({msg: "Data Berhasil Dimasukkan"});
        } else {
            res.status(400).json({msg: "Tidak terdapat data penanaman yang sedang berlangsung"});
        }

        
        
    } catch(error){
        console.log(error.message);
    }
}

exports.saveSCN = saveSCN;
exports.getSCNbyIdTanam = getSCNbyIdTanam;
exports.getSCNbyWaterPumpOn = getSCNbyWaterPumpOn;