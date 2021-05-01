
'use strict';


const express = require('express');
const PORT = 5000;
const HOST = 'localhost';
const tables  = require('./models');
const Perfil = tables.Perfil;
// App
const app = express();
app.use(express.json());
//app.use(express.urlencoded({ extended: true }));


//listar todas as informações de todos os perfis
app.get('/', (req, res) => {
  Perfil.findAll().then((data) => {
    res.json(data);
  });
});


//Lista todas as pessoas (email e nome) formadas em um determinado curso;
app.get('/curso/:curso', (req, res) => {
  
  Perfil.findAll({
    attributes: ['email',  'nome'],
    where: {
      formacao_academica: req.params['curso']
    }
  }).then((data) => {
    res.json(data);
  });
});

//lista todas as pessoas (email e nome) que possuam uma determinada habilidade;
// app.get('/habilidade/:habilidade', (req, res) => {
  
//   Perfil.findAll({
//     attributes: ['email',  'nome'],
//     where: {
//       habilidades:{
//         [Op.in]:[req.params['curso']]
//     }
//   }).then((data) => {
//     res.json(data);
//   });
// });

//lista todas as pessoas (email, nome e curso) formadas em um determinado ano;
app.get('/ano/:ano', (req, res) => {
  
  Perfil.findAll({
    attributes: ['email',  'nome', 'formacao_academica'],
    where: {
      ano_formatura: req.params['ano']
    }
  }).then((data) => {
    res.json(data);
  });
});

//dado o email de um perfil, retornar suas informações
app.get('/email/:email', (req, res) => {
  
  Perfil.findAll({
    where: {
      email: req.params['email']
    }
  }).then((data) => {
    res.json(data);
  });
});




//Create
app.post('/', function(req, res) {
    console.log(req.body);

  const resultadoCreate = Perfil.create({
        email: req.body.email,
        nome: req.body.nome,
        residencia: req.body.residencia,
        formacao_academica: req.body.formacao_academica,
        ano_formatura : req.body.ano_formatura,
        habilidades:req.body.habilidades,
        experiencias:req.body.experiencias,
  })

  //Delete
  app.get('/delete/:email', (req, res) =>{
    Perfil.destroy({
      where: {
        email: req.params['email']
      }
    }).then((data) => {
      res.json(data);
    });

  })
  //console.log(resultadoCreate);
  res.send("Deleted");
});






app.listen(PORT, () => {
  //   if (err) {
  //     return console.error(err);
  //   }
    return console.log(`server is listening on ${PORT}`);
  });


/*
db.sequelize.sync({ force: true })//Depois trocar
  .then(()=>{
    User.create({ firstName: 'user1' }),
    User.create({ firstName: 'user2' })
  });
console.log("All models were synchronized successfully.");

https://reyhanhamidi.medium.com/sequelize-automatic-database-migration-and-seeding-on-heroku-fb88cf09573b
*/
/*
const { Pool, Client } = require('pg');
const dbClient = new Client({
  user: 'postgres',
  host: 'localhost',
  database: 'postgres',
  password: 'test',
  port: 5432,
});


dbClient.connect();
dbClient.query('SELECT * from Example', (err, res) => {
    console.log(err, res);
    dbClient.end();
});
*/