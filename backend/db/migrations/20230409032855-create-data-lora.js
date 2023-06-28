'use strict';
/** @type {import('sequelize-cli').Migration} */
module.exports = {
  async up(queryInterface, Sequelize) {
    await queryInterface.createTable('data_loras', {
      id: {
        allowNull: false,
        autoIncrement: true,
        primaryKey: true,
        type: Sequelize.INTEGER
      },
      son_temp: {
        type: Sequelize.FLOAT
      },
      son_humi: {
        type: Sequelize.FLOAT
      },
      son_soil: {
        type: Sequelize.FLOAT
      },
      son_count: {
        type: Sequelize.INTEGER
      },
      son_time: {
        type: Sequelize.BIGINT
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
    await queryInterface.dropTable('data_loras');
  }
};