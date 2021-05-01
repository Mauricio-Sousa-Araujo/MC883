'use strict';
const {
  Model
} = require('sequelize');
module.exports = (sequelize, DataTypes) => {
  class Perfil extends Model {
    /**
     * Helper method for defining associations.
     * This method is not a part of Sequelize lifecycle.
     * The `models/index` file will call this method automatically.
     */
    static associate(models) {
      // define association here
    }
  };
  Perfil.init({
    email:{ type:DataTypes.STRING,
            primaryKey: true,
            allowNull: false
          },
    nome: DataTypes.STRING,
    residencia : DataTypes.STRING,
    formacao_academica : DataTypes.STRING,
    ano_formatura: DataTypes.DATE,
    habilidades:DataTypes.ARRAY(DataTypes.TEXT),
    experiencias: DataTypes.ARRAY(DataTypes.TEXT),
    createdAt: DataTypes.DATE,
    updatedAt: DataTypes.DATE

  }, {
    sequelize,
    modelName: "Perfil",
    freezeTableName: true,
  });
  return Perfil;
};