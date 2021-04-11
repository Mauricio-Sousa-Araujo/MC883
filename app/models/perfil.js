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
    email: DataTypes.STRING,
    nome: DataTypes.STRING,
    residencia: DataTypes.STRING,
    formacao_academica : DataTypes.STRING
    
  }, {
    sequelize,
    modelName: 'Perfil',
  });
  return Perfil;
};

//ano_formatura: DataTypes.date
//habilidades : Sequelize.ARRAY(DataTypes.STRING),
//experiencias : Sequelize.ARRAY(DataTypes.STRING)
