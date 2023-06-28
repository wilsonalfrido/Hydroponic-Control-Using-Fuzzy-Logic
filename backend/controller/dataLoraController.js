const { data_lora } = require("../db/models/index.js");
const path = require("path");
const data_exporter = require("json2csv").Parser ;

const getLoraData = async(req,res)=>{
    try{
        var file_header = ['id','son_temp','son_humi','son_soil','son_count','son_time','hariKe'];
        const data = await data_lora.findAll(
            {
                attributes:['id','son_temp','son_humi','son_soil','son_count','son_time','hariKe']
            }
        );
        var mysql_data = JSON.parse(JSON.stringify(data));
        var json_data = new data_exporter({file_header});
        var csv_data = json_data.parse(mysql_data);

        res.setHeader("Content-Type","text/csv");
        res.setHeader("Content-Disposition","attachment;filename=dataLora.csv");
        // res.send(csv_data);
        res.status(200).end(csv_data);

    } catch(error) {
        res.status(400).json({msg:error.message});
    }
}

const getLoraDataByHari = async(req,res)=>{
    try{
        var file_header = ['id','son_temp','son_humi','son_soil','son_count','son_time','hariKe'];
        const data = await data_lora.findAll(
            {
                where:{
                    hariKe:req.params.hariKe
                },attributes:['id','son_temp','son_humi','son_soil','son_count','son_time','hariKe']
            }
        );
        if(data != null){
            var mysql_data = JSON.parse(JSON.stringify(data));
            var json_data = new data_exporter({file_header});
            var csv_data = json_data.parse(mysql_data);
    
            res.setHeader("Content-Type","text/csv");
            res.setHeader("Content-Disposition",`attachment;filename=dataLoraHari_${req.params.hariKe}.csv`);
            // res.send(csv_data);
            res.status(200).end(csv_data);
        } else {
            res.status(200).json({msg:`Data hari ke-${req.params.hariKe} tidak ditemukan`});
        }


    } catch(error) {
        res.status(400).json({msg:error.message});
    }
}

const saveLoraData = async(req,res)=>{
    const son_temp = req.body.son_temp;
    const son_humi = req.body.son_humi;
    const son_soil = req.body.son_soil;
    const son_count = req.body.son_count;
    const son_time = req.body.son_time;

    const lastData = await data_lora.findAll({
        // order: [[ 'id', 'ASC' ]],   
        where:{
            id:1
        }
    });

    var date1 = new Date(lastData[0].createdAt);
    var date2 = new Date();

    var Difference_In_Time = date2.getTime() - date1.getTime();
  
	// hitung jml hari antara dua tanggal
	const Difference_In_Days = Difference_In_Time / (1000 * 3600 * 24); 
    


    try{
        const data = await data_lora.create({
            son_temp:son_temp,
            son_humi:son_humi,
            son_soil:son_soil,
            son_count:son_count,
            son_time:son_time,
            hariKe:Difference_In_Days + 1
        });
        res.status(200).json({msg:"Data berhasil disimpan"});
    } catch(error) {
        res.status(400).json({msg:error.message});
    }
}





exports.getLoraData = getLoraData;
exports.getLoraDataByHari = getLoraDataByHari;
exports.saveLoraData = saveLoraData;
