'use strict';
/** @type {import('sequelize-cli').Migration} */
module.exports = {
  async up(queryInterface, Sequelize) {
    await queryInterface.createTable('data_history_pompas', {
      id: {
        allowNull: false,
        autoIncrement: true,
        primaryKey: true,
        type: Sequelize.INTEGER
      },
      offset_ec: {
        type: Sequelize.FLOAT
      },
      ec: {
        type: Sequelize.FLOAT
      },
      ph: {
        type: Sequelize.FLOAT
      },
      water_level: {
        type: Sequelize.FLOAT
      },
      water_temp: {
        type: Sequelize.FLOAT
      },
      ab_dur: {
        type: Sequelize.FLOAT
      },
      wp_dur: {
        type: Sequelize.FLOAT
      },
      set_water_level: {
        type: Sequelize.FLOAT
      },
      set_ec: {
        type: Sequelize.FLOAT
      },
      hariKe: {
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
    await queryInterface.dropTable('data_history_pompas');
  }
};