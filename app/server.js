
'use strict';


const express = require('express');
const db = require('./models/index.js');
const { User } = require("./models");

//const user = require('./models/user.js');


// Constants
const PORT = 8080;
const HOST = '0.0.0.0';

// App
const app = express();
app.get('/', (req, res) => {
  res.send('Hello World');
});

//Recriamos a tabela
db.sequelize.sync({ force: true })//Depois trocar
  .then(()=>{
    User.create({ firstName: 'user1' }),
    User.create({ firstName: 'user2' })
  });
console.log("All models were synchronized successfully.");


app.listen(PORT, HOST);
console.log(`Running on http://${HOST}:${PORT}`);


/*
import { Sequelize } from 'sequelize';

// This will create an in-memory sqlite db
const sequelize = new Sequelize('sqlite::memory:', {
  logging: sequelizeLogger
});

await sequelize
  .sync({ force: true })
  .then(() => {
    // seed db
    Users.create({ username: 'user1' })
  });
*/

/*

https://reyhanhamidi.medium.com/sequelize-automatic-database-migration-and-seeding-on-heroku-fb88cf09573b
*/