#include <stdio.h> /* printf, sprintf */
#include <stdlib.h> /* exit */
#include <unistd.h> /* read, write, close */
#include <string.h> /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h> /* struct hostent, gethostbyname */

#define PORT 8001
#define QUEUE 20
#define MAX 4096

int requestAPI(char *message_request, char *response );

void error(const char *msg) { perror(msg); exit(0); }


int main(int argc, char **argv)
{
    struct hostent *server;
    int sock_fd, new_fd, sock_api;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;
    char message[MAX];
    int bytes, received, total;
    char request[1024];

    /* connect the socket */
    sock_fd = socket (AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1)  printf("Could not create socket");

    
    bzero(&servaddr, sizeof(servaddr));
    //IPV4
    servaddr.sin_family = AF_INET;
    // Endereço Internet, IP = Local
    servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
    //Porta Network Byte Order
    servaddr.sin_port = htons (PORT);

    //Vincula o socket a um endereço IP e e porta  
    if(bind(sock_fd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0)  printf("bind failed");

    //listen até no máximo QUEUE 
    listen(sock_fd, QUEUE);


    for ( ; ; ) {
        clilen = sizeof(cliaddr);
        new_fd = accept(sock_fd, (struct sockaddr *) &cliaddr, &clilen);        
        
        if (new_fd<0) perror("accept request failed");
        if(!fork() && new_fd>=0){//Criamos novo processo

            close(sock_fd);
            /* receive the request  */
            memset(request, 0, sizeof(request));
            total = sizeof(request)-1;
            received = 0;
            read(new_fd, request, total);
            
            /* send the request to api and take the received  */
            memset(message, 0, sizeof(message));
            sock_api = requestAPI(request, message);
            close(sock_api);

            printf("Message received: \n%s\n", message);
            write(new_fd , message , strlen(message));    
            close(new_fd); 
            exit(0);//Saídos do Processo
        }
        close(new_fd);

    }
    
}


int requestAPI(char *message_request, char *response ){

    /* Port where the api nodejs listen and ip address*/
    int portno =        5000; 
    char *host =        "localhost";

    struct hostent *server;
    struct sockaddr_in serv_addr;
    int sockfd, bytes, sent, received, total;
    
    printf("Request:\n%s\n", message_request);

    /* create the socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");

    /* lookup the ip address */
    server = gethostbyname(host);
    if (server == NULL) error("ERROR, no such host for database");

    /* fill in the structure */
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    /* connect the socket */
    if (connect(sockfd, (struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    /* send the request */
    total = strlen(message_request);
    sent = 0;
    write(sockfd, message_request, total);
    
    /* receive the request */
    memset(response, 0, sizeof(response));
    total = sizeof(response)-1;
    received = 0;
    do{
        bytes = read(sockfd, response+received, total);
        received+=bytes;
        
    }while(bytes>0);

    return sockfd; 
    
}

