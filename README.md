# MC883
O diretório corrente(raiz) apresenta 3 diretórios centrais: app, db e socket. Em db está localizado os arquivos relacionados a banco de dados;
em socket o client e o server implementados em C. Em app está contido uma api em nodejs que serve de camada entre server(presente no diretório socket) e o banco de dados.
Então o fluxo para a requisição é da seguinte forma: o usuário faz um request através client enviando para o server. O server pega essa requisição, empacota e manda para api nodejs. A api nodejs se conecta e pega do banco de dados e devolve para server e o server devolve para o client. Criamos tal api, pois ela  facilita a comunicação com banco de dados(ela possui uma linguagem de comunicação com banco de dados mais simplifica que sql). Ao invés de fazermos uma requisição do server para o database diretamente, utilizamos ela como interface. 

Utilizamos estruturas de containers.










gcc -o projeto01 projeto01.c
https://stackoverflow.com/questions/22077802/simple-c-example-of-doing-an-http-post-and-consuming-the-response

docker-compose build --no-cache && docker-compose up

docker rmi $(docker images -f "dangling=true" -q)

sudo apt-get install postgresql postgresql-client postgresql-contrib libpq-dev

gcc connect_db.c -L /path/to/postgresql/lib -l pq

g++ -I /usr/include/postgresql -L /usr/lib/postgresql/12/lib/ -o connect_db connect_db.c -lpq