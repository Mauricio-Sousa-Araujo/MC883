CREATE TABLE "Perfil" (  
    email       varchar(40) PRIMARY KEY, 
    nome        varchar(200) NOT NULL,
    residencia  varchar,
    formacao_academica  varchar(40),
    ano_formatura  date,
    habilidades   varchar[],
    experiencias  varchar[],
    "createdAt" Date,
    "updatedAt" Date
);

