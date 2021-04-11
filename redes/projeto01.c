
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#include <arpa/inet.h> 
#include <errno.h>


#define PORT 8080
#define QUEUE 3
#define MAXLINE 50
void str_echo(int sockfd);


int main(int argc, char **argv)
{
    int sock_fd, new_fd;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;
    char *message;

    //Criamos o socket
    sock_fd = socket (AF_INET, SOCK_STREAM, 0);

    if (sock_fd == -1)  printf("Could not create socket");

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

