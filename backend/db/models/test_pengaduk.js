'use strict';
const {
  Model
} = require('sequelize');
module.exports = (sequelize, DataTypes) => {
  class test_pengaduk extends Model {
    /**
     * Helper method for defining associations.
     * This method is not a part of Sequelize lifecycle.
     * The `models/index` file will call this method automatically.
     */
    static associate(models) {
      // define association here
    }
  }
  test_pengaduk.init({
    ph: DataTypes.FLOAT,
    ec: DataTypes.FLOAT,
    water_temp: DataTypes.FLOAT,
    ket: DataTypes.INTEGER,
    pengaduk: DataTypes.INTEGER,
    hariKe: DataTypes.INTEGER 
  }, {
    sequelize,
    modelName: 'test_pengaduk',
  });
  return test_pengaduk;
};