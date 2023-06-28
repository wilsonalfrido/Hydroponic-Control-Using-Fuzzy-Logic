'use strict';
const {
  Model
} = require('sequelize');
module.exports = (sequelize, DataTypes) => {
  class dataTanam extends Model {
    /**
     * Helper method for defining associations.
     * This method is not a part of Sequelize lifecycle.
     * The `models/index` file will call this method automatically.
     */
    static associate(models) {
      // define association here
      this.hasMany(models.dataSCN,{
        foreignKey: 'id_dataTanam',
        onDelete: 'SET NULL',
        onUpdate: 'CASCADE'
      })
    }
  }
  dataTanam.init({
    tanggal_semai: DataTypes.DATE,
    tanggal_tanam: DataTypes.DATE,
    tanggal_panen: DataTypes.DATE,
    status: DataTypes.INTEGER
  }, {
    sequelize,
    modelName: 'dataTanam',
  });
  return dataTanam;
};