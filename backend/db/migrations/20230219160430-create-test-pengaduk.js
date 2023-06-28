'use strict';
/** @type {import('sequelize-cli').Migration} */
module.exports = {
  async up(queryInterface, Sequelize) {
    await queryInterface.createTable('test_pengaduks', {
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
      ket: {
        type: Sequelize.INTEGER
      },
      pengaduk: {
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
    await queryInterface.dropTable('test_pengaduks');
  }
};