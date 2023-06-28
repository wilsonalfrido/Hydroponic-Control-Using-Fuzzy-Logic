'use strict';
const {
  Model
} = require('sequelize');
module.exports = (sequelize, DataTypes) => {
  class data_setting_flc extends Model {
    /**
     * Helper method for defining associations.
     * This method is not a part of Sequelize lifecycle.
     * The `models/index` file will call this method automatically.
     */
    static associate(models) {
      // define association here
    }
  }
  data_setting_flc.init({
    offset_ec: DataTypes.FLOAT,
    ec: DataTypes.FLOAT,
    ph: DataTypes.FLOAT,
    water_level: DataTypes.FLOAT,
    water_temp: DataTypes.FLOAT,
    ab_dur: DataTypes.FLOAT,
    wp_dur: DataTypes.FLOAT,
    set_water_level: DataTypes.FLOAT,
    set_ec: DataTypes.FLOAT,
    status: DataTypes.INTEGER
  }, {
    sequelize,
    modelName: 'data_setting_flc',
  });
  return data_setting_flc;
};