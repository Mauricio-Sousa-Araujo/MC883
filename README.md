########## Explicação arquivos ###########
O diretório corrente(raiz) apresenta 3 diretórios centrais: app, db e socket. Em db está localizado os arquivos relacionados ao banco de dados;
em socket o client e o server implementados em C. Em app está contido uma api em nodejs que serve de camada intermediária entre o server(presente no diretório /socket) e o banco de dados.
Então o fluxo para a requisição é da seguinte forma: o usuário faz um request através client enviando para o server. O server pega essa requisição, empacota e manda para api nodejs. A api nodejs se conecta ao banco de dados e devolve para api que devolve para o server  e esse devolve para o client. Criamos tal api, pois ela  facilita a comunicação com banco de dados(ela possui uma linguagem de comunicação com banco de dados mais simplifica que sql). Ao invés de fazermos uma requisição do server para o database diretamente, utilizamos ela como interface. 


########## Instruções para subir o banco e dados e rodar o server ###########
Para subir o banco de dados, primeiro baixe o docker e rode no diretório raiz do projeto o seguinte comando:

- docker-compose build --no-cache && docker-compose up

Em seguida vá até o diretório /socket e execute o server:

- ./server.o

Por fim, abra novamente outro aba de terminal e execute o comando no diretório /socket

- ./client.o args


######Por que docker?######

De forma breve, utilizamos docker para encapsular nossas aplicações(api e database ) em containers para ser independente .... 

#Comandos utilizados no projeto para compilar
- gcc -o client.o client.c
- gcc -o server.o server.c
- docker-compose build --no-cache && docker-compose up          #Subir aplicação
- docker rmi $(docker images -f "dangling=true" -q)             #Excluir execessos de container denominados none
- docker container prune 


#########################Casos de Uso###########################
##GETs
#dado o email de um perfil, retornar suas informações
- ./client.o localhost 8001 GET /email/maria_silva@gmail.com

#lista todas as pessoas (email, nome e curso) formadas em um determinado ano;
- ./client.o localhost 8001 GET /ano/2015

#Lista todas as pessoas (email e nome) formadas em um determinado curso   
- ./client.o localhost 8001 GET /curso/ "curso=Sistemas de Informação"

#Lista todas as informações de todos os perfis
- ./client.o "" "" GET /

#Lista todas as pessoas (email e nome) que possuam uma determinada habilidade;
- ./client.o localhost 8001 GET /habilidade/ "habilidade=Jurista"

##POST
#Cadastrar um novo perfil utilizando o email como identificador
- ./client.o "" "" POST / "email=mauricio_ploc@hotmail.com" "nome=Mauricio de Sousa" "habilidades=Análise de Dados; Internet das Coisas; Computação em Nuvem"
- ./client.o "" "" POST / "email=andre_ploc@hotmail.com" "nome=André de Sousa" "experiencias=Trabalhei 10 anos com Pandas; Trabalhei 5 anos com Python"

##PUT
#Acrescenta uma nova experiência profissional em um perfil
- ./client.o "" "" PUT /experiencia/mauricio_ploc@hotmail.com "experiencia=Trabalhei 10 anos com ferramentas"

##Delete
#Deleta um perfil pelo email
- ./client.o localhost 8001 DELETE /delete/mauricio_ploc@hotmail.com