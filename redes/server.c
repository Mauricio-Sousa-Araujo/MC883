#include <stdio.h> /* printf, sprintf */
#include <stdlib.h> /* exit */
#include <unistd.h> /* read, write, close */
#include <string.h> /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h> /* struct hostent, gethostbyname */



#define PORT 8080
#define QUEUE 3
#define MAXLINE 50
void str_echo(int sockfd);


int main(int argc, char **argv)
{
    int portno =        5000;
    char *host =        "localhost";
    char *message_fmt = "POST /apikey=%s&command=%s HTTP/1.0\r\n\r\n";
    
    struct hostent *server;

    int sock_fd, new_fd;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;
    char *message;

    //Criamos o socket
    sock_fd = socket (AF_INET, SOCK_STREAM, 0);

    if (sock_fd == -1)  printf("Could not create socket");

    server = gethostbyname(host);
    if (server == NULL) herror("ERROR, no such host");
    //if (server == NULL) perror("ERROR, no such host");

    //bzero(&servaddr, sizeof(servaddr));
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
 
        
        if (new_fd<0) perror("accept failed");
        message = "Hello Client , I have received your connection. But I have to go now, bye\n";
	    write(new_fd , message , strlen(message));
        close(new_fd); /* parent closes connected socket */
    }

    

    
}




/*
    if ( (childpid = fork()) == 0) { // child process 
            close(sock_fd); // close listening socket 
            //str_echo(new_fd); // process the request 
            exit (0);
    }
*/

