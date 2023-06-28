'use strict';
const {
  Model
} = require('sequelize');
module.exports = (sequelize, DataTypes) => {
  class User extends Model {
    /**
     * Helper method for defining associations.
     * This method is not a part of Sequelize lifecycle.
     * The `models/index` file will call this method automatically.
     */
    static associate(models) {
      // define association here
    }
  }
  User.init({
    ph: DataTypes.FLOAT,
    ec: DataTypes.FLOAT,
    water_temp: DataTypes.FLOAT,
    water_level: DataTypes.FLOAT,
    hariKe: DataTypes.INTEGER
  }, {
    sequelize,
    modelName: 'User',
  });
  return User;
};