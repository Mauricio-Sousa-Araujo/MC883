CREATE TABLE Perfil (
    email       varchar(40) CONSTRAINT firstkey PRIMARY KEY, 
    nome        varchar(200) NOT NULL,
    residencia  varchar,
    formacao_academica  varchar(40) NOT NULL,
    ano_formatura  date,
    habilidades   varchar[],
    experiencias  varchar[]
);

