'use strict';
const {
  Model
} = require('sequelize');
module.exports = (sequelize, DataTypes) => {
  class data_history_pompa extends Model {
    /**
     * Helper method for defining associations.
     * This method is not a part of Sequelize lifecycle.
     * The `models/index` file will call this method automatically.
     */
    static associate(models) {
      // define association here
    }
  }
  data_history_pompa.init({
    offset_ec: DataTypes.FLOAT,
    ec: DataTypes.FLOAT,
    ph: DataTypes.FLOAT,
    water_level: DataTypes.FLOAT,
    water_temp: DataTypes.FLOAT,
    ab_dur: DataTypes.FLOAT,
    wp_dur: DataTypes.FLOAT,
    set_water_level: DataTypes.FLOAT,
    set_ec: DataTypes.FLOAT,
    hariKe: DataTypes.INTEGER,
    id_dataTanam: DataTypes.INTEGER,
    flc_mode: DataTypes.INTEGER
    
  }, {
    sequelize,
    modelName: 'data_history_pompa',
  });
  return data_history_pompa;
};