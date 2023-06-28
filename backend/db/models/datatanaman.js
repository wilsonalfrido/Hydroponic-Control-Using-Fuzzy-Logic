'use strict';
const {
  Model
} = require('sequelize');
module.exports = (sequelize, DataTypes) => {
  class dataTanaman extends Model {
    /**
     * Helper method for defining associations.
     * This method is not a part of Sequelize lifecycle.
     * The `models/index` file will call this method automatically.
     */
    static associate(models) {
      // define association here
    }
  }
  dataTanaman.init({
    nama: DataTypes.STRING,
    phMin: DataTypes.FLOAT,
    phMax: DataTypes.FLOAT,
    ecMin: DataTypes.FLOAT,
    ecMax: DataTypes.FLOAT
  }, {
    sequelize,
    modelName: 'dataTanaman',
  });
  return dataTanaman;
};