'use strict';
const {
  Model
} = require('sequelize');
module.exports = (sequelize, DataTypes) => {
  class data_uji_flc extends Model {
    /**
     * Helper method for defining associations.
     * This method is not a part of Sequelize lifecycle.
     * The `models/index` file will call this method automatically.
     */
    static associate(models) {
      // define association here
    }
  }
  data_uji_flc.init({
    ph: DataTypes.FLOAT,
    ec: DataTypes.FLOAT,
    water_temp: DataTypes.FLOAT,
    water_level: DataTypes.FLOAT,
    ab_pump: DataTypes.INTEGER,
    water_pump: DataTypes.INTEGER,
    id_flc: DataTypes.INTEGER
  }, {
    sequelize,
    modelName: 'data_uji_flc',
  });
  return data_uji_flc;
};