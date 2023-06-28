'use strict';

const fs = require('fs');
const path = require('path');
const Sequelize = require('sequelize');
const process = require('process');
const basename = path.basename(__filename);
const env = process.env.NODE_ENV || 'development';
const config = require(__dirname + '/../../config/config.json')[env];
const db = {};

const test_pengaduk = require('./test_pengaduk.js');
const data_lora = require('./data_lora.js');
const dataTanam = require('./datatanam.js');
const dataSCN = require('./datascn.js');
const data_setting_flc = require('./data_setting_flc.js');
const data_uji_flc = require('./data_uji_flc.js');
const data_history_pompa = require('./data_history_pompa.js');

let sequelize;
if (config.use_env_variable) {
  sequelize = new Sequelize(process.env[config.use_env_variable], config);
} else {
  sequelize = new Sequelize(config.database, config.username, config.password, config);
}

fs
  .readdirSync(__dirname)
  .filter(file => {
    return (
      file.indexOf('.') !== 0 &&
      file !== basename &&
      file.slice(-3) === '.js' &&
      file.indexOf('.test.js') === -1
    );
  })
  .forEach(file => {
    const model = require(path.join(__dirname, file))(sequelize, Sequelize.DataTypes);
    db[model.name] = model;
  });

Object.keys(db).forEach(modelName => {
  if (db[modelName].associate) {
    db[modelName].associate(db);
  }
});

db.sequelize = sequelize;
db.Sequelize = Sequelize;

db.test_pengaduk = test_pengaduk(sequelize,Sequelize);
db.data_lora = data_lora(sequelize,Sequelize);
db.dataTanam = dataTanam(sequelize,Sequelize);
db.dataSCN = dataSCN(sequelize,Sequelize);
db.data_setting_flc = data_setting_flc(sequelize,Sequelize);
db.data_uji_flc = data_uji_flc(sequelize,Sequelize);
db.data_history_pompa = data_history_pompa(sequelize,Sequelize);

module.exports = db;
