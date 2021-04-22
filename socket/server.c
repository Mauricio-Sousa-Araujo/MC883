#include <stdio.h> /* printf, sprintf */
#include <stdlib.h> /* exit */
#include <unistd.h> /* read, write, close */
#include <string.h> /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h> /* struct hostent, gethostbyname */



#define PORT 8001
#define QUEUE 20
#define MAXLINE 50


char * requestAPI(char *message_request );

void error(const char *msg) { perror(msg); exit(0); }


int main(int argc, char **argv)
{
    
    struct hostent *server;

    int sock_fd, new_fd;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;
    char *message;

    pid_t pid;

    int bytes, received, total;
    char request[1024];

    //Criamos o socket
    sock_fd = socket (AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1)  printf("Could not create socket");

    
    bzero(&servaddr, sizeof(servaddr));
    //IPV4
    servaddr.sin_family = AF_INET;
    // Endereço Internet, IP = Local
    servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
    //Porta Network Byte Order
    servaddr.sin_port = htons (PORT);

    //Vincula o socket(sock_fd) a um endereço IP e e porta  
    if(bind(sock_fd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0)  printf("bind failed");

    
    listen(sock_fd, QUEUE);


    for ( ; ; ) {
        clilen = sizeof(cliaddr);
        new_fd = accept(sock_fd, (struct sockaddr *) &cliaddr, &clilen);        
        if (new_fd<0) perror("accept request failed");
        
         if (!fork()) { // this is the child process
            close(sock_fd); // child doesn't need the listener

            /* receive the request  */
            memset(request, 0, sizeof(request));
            total = sizeof(request)-1;
            received = 0;

            do {
                bytes = read(new_fd, request+received, total-received);
                
                if (bytes < 0)
                    error("ERROR reading response from socket");
                if (bytes == 0)
                    break;
                received+=bytes;

            } while (received < total);
            printf("%s", request);
            if (received == total){
            
                error("ERROR storing complete response from socket");
            }
            if (!fork()) { // this is the child process
                message = requestAPI(request);
            }
            printf("%s", message);
            
            write(new_fd , message , strlen(message));     
            close(new_fd);
   
            exit(0);
             /* parent closes connected socket */
         }
    }

    

    
}


char * requestAPI(char *message_request ){

      /* first what are we going to send and where are we going to send it? */
    int portno =        8001; //Porta onde a api(interface database) está escutando
    char *host =        "localhost";
    //char *message_request = "GET / HTTP/1.0\r\n\r\n";

    struct hostent *server;
    struct sockaddr_in serv_addr;
    int sockfd, bytes, sent, received, total;
    char *message;
    char *response = malloc (sizeof (char) * 4096);

   
    /* fill in the parameters */
    message=message_request;
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
    total = strlen(message);
    sent = 0;
    do {
        bytes = write(sockfd,message+sent,total-sent);
        if (bytes < 0)
            error("ERROR writing message to socket");
        if (bytes == 0)
            break;
        sent+=bytes;
    } while (sent < total);

    /* receive the response */
    memset(response, 0, sizeof(response));
    total = sizeof(response)-1;
    received = 0;

    do {
        bytes = read(sockfd, response+received, total-received);
        
        if (bytes < 0)
            error("ERROR reading response from socket");
        if (bytes == 0)
            break;
        received+=bytes;

    } while (received < total);

    if (received == total)
        error("ERROR storing complete response from socket147852");
    
    /* close the socket */
    close(sockfd);

    /* process response */
    return response ;


}

