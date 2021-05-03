CREATE TABLE "Perfil" (  
    email       varchar(40) PRIMARY KEY, 
    nome        varchar(200) NOT NULL,
    residencia  varchar,
    formacao_academica  varchar(40),
    ano_formatura  varchar(5),
    habilidades   text[],
    experiencias  text[],
    "createdAt" date not null default CURRENT_DATE,
    "updatedAt" date not null default CURRENT_DATE
);
