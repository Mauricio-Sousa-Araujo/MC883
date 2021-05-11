
'use strict';
var Sequelize = require ('sequelize');
const express = require('express');
const PORT = 5000;
const HOST = 'localhost';
const tables  = require('./models');
const Perfil = tables.Perfil;
// App
const app = express();
app.use(express.json());


//listar todas as informações de todos os perfis
app.get('/', (req, res) => {
  Perfil.findAll().then((data) => {
    res.json(data);
  }).catch(err =>
    res.send("Erro")
  );
});

//Acrescenta uma nova experiência profissional em um perfil;
app.put('/experiencia/:email', (req, res) => {
  
  Perfil.update(
    {experiencias: Sequelize.fn('array_append', Sequelize.col('experiencias'), req.body.experiencia)},
    {
      where: {
        email: req.params['email']
    }
  }).then((data) => {
    res.json(data);
  }).catch(err =>
    res.send("Erro ao atualizar")
  );
});



//Lista todas as pessoas (email e nome) formadas em um determinado curso;
app.get('/curso/', (req, res) => {
  
  Perfil.findAll({
    attributes: ['email',  'nome'],
    where: {
      formacao_academica: req.body.curso
    }
  }).then((data) => {
    res.json(data);
  }).catch(err =>
    res.send("Erro")
  );
});

//lista todas as pessoas (email e nome) que possuam uma determinada habilidade;
app.get('/habilidade/', (req, res) => {  
  Perfil.findAll({
    attributes: ['email',  'nome'],
    where: {
      habilidades:{
        [Sequelize.Op.contains]:[req.body.habilidade]
      }
    }
    }).then((data) => {
        res.json(data);
  }).catch(err =>
    res.send("Erro")
  );
});

//lista todas as pessoas (email, nome e curso) formadas em um determinado ano;
app.get('/ano/:ano', (req, res) => {
  
  Perfil.findAll({
    attributes: ['email',  'nome', 'formacao_academica'],
    where: {
      ano_formatura: req.params['ano']
    }
  }).then((data) => {
    res.json(data);
  }).catch(err =>
    res.send("Erro")
  );
});

//Dado o email de um perfil, retornar suas informações
app.get('/email/:email', (req, res) => {
  
  Perfil.findAll({
    where: {
      email: req.params['email']
    }
  }).then((data) => {
    res.json(data);
  }).catch(err =>
    res.send("Erro")
  );
});

//Create
app.post('/', (req, res) => {

  const resultadoCreate = Perfil.create({
        email: req.body.email,
        nome: req.body.nome,
        residencia: req.body.residencia,
        formacao_academica: req.body.formacao_academica,
        ano_formatura : req.body.ano_formatura,
        habilidades:req.body.habilidades,
        experiencias:req.body.experiencias

  }).then((data) => {
    res.send("Successfully inserted")
  }).catch(err =>
    res.send("Erro ao inserir")
  );
});

//Deleta um perfil pelo email
app.delete('/delete/:email', (req, res) =>{
  Perfil.destroy({
    where: {
      email: req.params['email']
    }
  }).then((data) => {
    res.json(data);
  }).catch(err =>
    res.send("Erro ao deletar")
  );
});
  
const s=app.listen(PORT, () => {
    return console.log(`server is listening on ${PORT}`);
  });
  
  s.keepAliveTimeout = 1000;// Esperamos 1 segundo


