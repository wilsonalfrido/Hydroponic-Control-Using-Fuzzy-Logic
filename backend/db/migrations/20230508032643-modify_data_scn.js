'use strict';

/** @type {import('sequelize-cli').Migration} */
module.exports = {
  async up (queryInterface, Sequelize) {
    /**
     * Add altering commands here.
     *
     * Example:
     * await queryInterface.createTable('users', { id: Sequelize.INTEGER });
     */
     return Promise.all([
      queryInterface.addColumn(
        'datascns',
        'ab_pump',
        {
          type: Sequelize.INTEGER,
          allowNull: true,
        },
      ),
      queryInterface.addColumn(
        'datascns',
        'water_pump',
        {
          type: Sequelize.INTEGER,
          allowNull: true,
        },
      ),
      queryInterface.removeColumn('datascns','waterPump'),
      queryInterface.removeColumn('datascns','duration'),
     ])
  },

  async down (queryInterface, Sequelize) {
    /**
     * Add reverting commands here.
     *
     * Example:
     * await queryInterface.dropTable('users');
     */
     return Promise.all([
      queryInterface.removeColumn('datascns','ab_pump'),
      queryInterface.removeColumn('datascns','water_pump'),
      queryInterface.addColumn(
        'datascns',
        'waterPump',
        {
          type: Sequelize.INTEGER,
          allowNull: true,
        },
      ),
      queryInterface.addColumn(
        'datascns',
        'duration',
        {
          type: Sequelize.INTEGER,
          allowNull: true,
        },
      ),
    ])
  }
};