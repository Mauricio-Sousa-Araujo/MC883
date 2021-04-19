
'use strict';


const express = require('express');
const PORT = 8080;
const HOST = '0.0.0.0';
const tables  = require('./models');
// App
const app = express();
app.get('/', (req, res) => {
  tables.Perfil.findAll().then(notes => res.json(notes));
 
});


app.listen(PORT, HOST);
console.log(`Running on http://${HOST}:${PORT}`);


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