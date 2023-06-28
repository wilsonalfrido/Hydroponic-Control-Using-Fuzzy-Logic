const { test_pengaduk } = require("../db/models/index.js");
const path = require("path");
const data_exporter = require("json2csv").Parser ;


const getSCN = async(req,res)=>{
    try{
        var file_header = ['id','ph','ec','water_temp','ket','pengaduk','createdAt','hariKe'];
        const data = await test_pengaduk.findAll(
            {
                attributes:['id','ph','ec','water_temp','ket','pengaduk','createdAt','hariKe']
            }
        );
        var mysql_data = JSON.parse(JSON.stringify(data));
        var json_data = new data_exporter({file_header});
        var csv_data = json_data.parse(mysql_data);

        res.setHeader("Content-Type","text/csv");
        res.setHeader("Content-Disposition","attachment;filename=data.csv");
        // res.send(csv_data);
        res.status(200).end(csv_data);

    } catch(error) {
        res.status(400).json({msg:error.message});
    }
} 

const saveSCN = async(req,res)=>{
    const ph = req.body.ph;
    const ec = req.body.ec;
    const water_temp = req.body.water_temp;
    const ket = req.body.ket;
    const pengaduk = req.body.pengaduk;

    const lastData = await test_pengaduk.findAll({
        // order: [[ 'id', 'ASC' ]],   
        where:{
            id:4499
        }
    });

    var date1 = new Date(lastData[0].createdAt);
    var date2 = new Date();

    var Difference_In_Time = date2.getTime() - date1.getTime();
  
	// hitung jml hari antara dua tanggal
	const Difference_In_Days = Difference_In_Time / (1000 * 3600 * 24);


    try{
        await test_pengaduk.create({ph:ph,ec:ec,water_temp:water_temp,ket:ket,pengaduk:pengaduk,hariKe:Difference_In_Days});
        res.status(201).json({msg: "Data Berhasil Dimasukkan"})
    } catch(error){
        console.log(error.message);
    }
}


exports.saveSCN = saveSCN;
exports.getSCN = getSCN;