#include <stdio.h> /* printf, sprintf */
#include <stdlib.h> /* exit */
#include <unistd.h> /* read, write, close */
#include <string.h> /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h> /* struct hostent, gethostbyname */


void error(const char *msg) { perror(msg); exit(0); }
char *buildMessage(int argc, char *argv[ ]);
int sizeMessage(int argc, char *argv[ ]);
int main(int argc,char *argv[])
{
    int portno = (argc>2 && atoi(argv[2])>0)?atoi(argv[2]):8001;
    char *host = ( argc>1 && strlen(argv[1])>0)?argv[1]:"localhost";
    //char *message_fmt = "GET /oi HTTP/1.0\r\n\r\n";//Hard code
    //char *message_fmt = "POST / HTTP/1.1 \r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 22\r\n\r\nid=dsads&name=mauricio";
    struct hostent *server;
    struct sockaddr_in serv_addr;
    int sockfd, bytes, sent, received, total;
    char *message, response[4096];

    message = buildMessage(argc, argv);
    /* What are we going to send? */
    printf("Request:\n%s\n",message);

    
    /* create the socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");

    /* lookup the ip address */
    server = gethostbyname(host);
    if (server == NULL) error("ERROR, no such host");

    /* fill in the structure */
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    memcpy(&serv_addr.sin_addr.s_addr,server->h_addr,server->h_length);

    /* connect the socket */

    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
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
        bytes = read(sockfd,response+received,total-received);
        
        if (bytes < 0)
            error("ERROR reading response from socket");
        if (bytes == 0)
            break;
        received+=bytes;

    } while (received < total);

    //if (received == total)
      //  error("ERROR storing complete response from socket");

    /* process response */
    printf("Response:\n%s\n", response);
    
    /* close the socket */
    close(sockfd);

    return 0;
}


char *buildMessage(int argc, char *argv[ ]){

    char  *message=malloc(sizeMessage(argc,argv));
    if(!strcmp(argv[3],"GET"))
    {
        if(argc>5)
            sprintf(message,"%s %s%s%s HTTP/1.1\r\n",
                strlen(argv[3])>0?argv[3]:"GET",               /* method         */
                strlen(argv[4])>0?argv[4]:"/",                 /* path           */
                strlen(argv[5])>0?"?":"",                      /* ?              */
                strlen(argv[5])>0?argv[5]:"");                 /* query string   */
        else
            sprintf(message,"%s %s HTTP/1.1\r\n",
                strlen(argv[3])>0?argv[3]:"GET",               /* method         */
                strlen(argv[4])>0?argv[4]:"/");                /* path           */
        for(int i=6;i<argc;i++)                                    /* headers        */
            {strcat(message,argv[i]);strcat(message,"\r\n");}
        strcat(message,"\r\n");                                /* blank line     */
    }
    else
    {
        sprintf(message,"%s %s HTTP/1.1\r\n",
            strlen(argv[3])>0?argv[3]:"POST",                  /* method         */
            strlen(argv[4])>0?argv[4]:"/");                    /* path           */
        for(int i=6;i<argc;i++)                                    /* headers        */
            {strcat(message,argv[i]);strcat(message,"\r\n");}
        if(argc>5){
            sprintf(message+strlen(message),"Content-Type: application/json\r\n");
            sprintf(message+strlen(message),"Content-Length: %ld\r\n",strlen(argv[5]));
            strcat(message,"\r\n");                                /* blank line     */
            for(int i=5;i<argc-1;i++){
                strcat(message,argv[i]);                                /* body           */
                strcat(message,"&");
            }
            strcat(message, argv[argc-1]);
        
        }
        
    }

    return message;
}

int sizeMessage(int argc, char *argv[ ]){

    int message_size=0;
    if(!strcmp(argv[3],"GET"))
    {
        message_size+=strlen("%s %s%s%s HTTP/1.1\r\n");        /* method         */
        message_size+=strlen(argv[3]);                         /* path           */
        message_size+=strlen(argv[4]);                         /* headers        */
        if(argc>5)
            message_size+=strlen(argv[5]);                     /* query string   */
        for(int i=6;i<argc;i++)                                    /* headers        */
            message_size+=strlen(argv[i])+strlen("\r\n");
        message_size+=strlen("\r\n");                          /* blank line     */
    }
    else
    {
        message_size+=strlen("%s %s HTTP/1.1\r\n");
        message_size+=strlen(argv[3]);                         /* method         */
        message_size+=strlen(argv[4]);                         /* path           */
        for(int i=6;i<argc;i++)                                    /* headers        */
            message_size+=strlen(argv[i])+strlen("\r\n");
        if(argc>5){
            message_size+=strlen("Content-Type: application/json\r\n");
            message_size+=strlen("Content-Length: %d\r\n")+10; /* content length */
        }
        message_size+=strlen("\r\n");                          /* blank line     */
        
        if(argc>5){
            for(int i=5;i<argc-1;i++){
                message_size+= strlen(argv[i]);    
                message_size+= strlen("&");                        /* body           */
            }      
            message_size+= strlen(argv[argc-1]);
        }            

    }

    /* allocate space for the message */
    return message_size;
}