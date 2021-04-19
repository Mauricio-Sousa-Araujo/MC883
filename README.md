# MC883
gcc -o projeto01 projeto01.c
https://stackoverflow.com/questions/22077802/simple-c-example-of-doing-an-http-post-and-consuming-the-response

docker-compose build --no-cache && docker-compose up

docker rmi $(docker images -f "dangling=true" -q)

sudo apt-get install postgresql postgresql-client postgresql-contrib libpq-dev

gcc connect_db.c -L /path/to/postgresql/lib -l pq

g++ -I /usr/include/postgresql -L /usr/lib/postgresql/12/lib/ -o connect_db connect_db.c -lpq