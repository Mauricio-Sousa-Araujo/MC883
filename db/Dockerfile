FROM postgres:latest
ADD ./scripts/init.sql docker-entrypoint-initdb.d
ADD ./scripts/script.sql docker-entrypoint-initdb.d
#EXPOSE 6666
