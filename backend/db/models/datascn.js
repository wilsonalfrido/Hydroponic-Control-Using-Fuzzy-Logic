'use strict';
const {
  Model
} = require('sequelize');
module.exports = (sequelize, DataTypes) => {
  class dataSCN extends Model {
    /**
     * Helper method for defining associations.
     * This method is not a part of Sequelize lifecycle.
     * The `models/index` file will call this method automatically.
     */
    static associate(models) {
      // define association here
      this.belongsTo(models.dataTanam);
    }
  }
  dataSCN.init({
    ph: DataTypes.FLOAT,
    ec: DataTypes.FLOAT,
    water_temp: DataTypes.FLOAT,
    water_level: DataTypes.FLOAT,
    hariKe: DataTypes.INTEGER,
    status: DataTypes.INTEGER,
    id_dataTanam: DataTypes.INTEGER,
    ab_pump: DataTypes.INTEGER,
    water_pump: DataTypes.INTEGER
  }, {
    sequelize,
    modelName: 'dataSCN',
  });
  return dataSCN;
};