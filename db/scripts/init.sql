CREATE TABLE "Perfil" (
    id SERIAL PRIMARY KEY,   
    email       varchar(40) , 
    nome        varchar(200) NOT NULL,
    residencia  varchar,
    formacao_academica  varchar(40) NOT NULL,
    ano_formatura  date,
    habilidades   varchar[],
    experiencias  varchar[],
    "createdAt" Date,
    "updatedAt" Date
);

