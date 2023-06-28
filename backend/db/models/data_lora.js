'use strict';
const {
  Model
} = require('sequelize');
module.exports = (sequelize, DataTypes) => {
  class data_lora extends Model {
    /**
     * Helper method for defining associations.
     * This method is not a part of Sequelize lifecycle.
     * The `models/index` file will call this method automatically.
     */
    static associate(models) {
      // define association here
    }
  }
  data_lora.init({
    son_temp: DataTypes.FLOAT,
    son_humi: DataTypes.FLOAT,
    son_soil: DataTypes.FLOAT,
    son_count: DataTypes.INTEGER,
    son_time: DataTypes.BIGINT,
    hariKe: DataTypes.INTEGER
  }, {
    sequelize,
    modelName: 'data_lora',
  });
  return data_lora;
};