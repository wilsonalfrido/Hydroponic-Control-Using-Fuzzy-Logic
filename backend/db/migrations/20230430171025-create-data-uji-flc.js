'use strict';
/** @type {import('sequelize-cli').Migration} */
module.exports = {
  async up(queryInterface, Sequelize) {
    await queryInterface.createTable('data_uji_flcs', {
      id: {
        allowNull: false,
        autoIncrement: true,
        primaryKey: true,
        type: Sequelize.INTEGER
      },
      ph: {
        type: Sequelize.FLOAT
      },
      ec: {
        type: Sequelize.FLOAT
      },
      water_temp: {
        type: Sequelize.FLOAT
      },
      water_level: {
        type: Sequelize.FLOAT
      },
      ab_pump: {
        type: Sequelize.INTEGER
      },
      water_pump: {
        type: Sequelize.INTEGER
      },
      id_flc: {
        type: Sequelize.INTEGER
      },
      createdAt: {
        allowNull: false,
        type: Sequelize.DATE
      },
      updatedAt: {
        allowNull: false,
        type: Sequelize.DATE
      }
    });
  },
  async down(queryInterface, Sequelize) {
    await queryInterface.dropTable('data_uji_flcs');
  }
};