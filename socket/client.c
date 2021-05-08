#include <stdio.h> /* printf, sprintf */
#include <stdlib.h> /* exit */
#include <unistd.h> /* read, write, close */
#include <string.h> /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h> /* struct hostent, gethostbyname */
#define MAX 4096
#define TIMEOUT 2



void error(const char *msg) { perror(msg); exit(0); }
char *buildMessage(int argc, char *argv[ ]);
void prettyPrint(char *response);
void printBody(char *message);
int main(int argc,char *argv[])
{
    int portno = (argc>2 && atoi(argv[2])>0)?atoi(argv[2]):8001;
    char *host = ( argc>1 && strlen(argv[1])>0)?argv[1]:"localhost";
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

    /* process response */
    printf("\nResponse:\n\n");
    prettyPrint(response);
    
    /* close the socket */
    close(sockfd);

    return 0;
}


char *buildMessage(int argc, char *argv[ ]){

    //char  *message=malloc(sizeMessage(argc,argv));
    char  *message=malloc(4096);
    char *json=malloc(4096);
    
    sprintf(message,"%s %s HTTP/1.1\r\n",
            argv[3],                                    /* method         */
            argv[4]);                                   /* path           */
    
    if(argc>5){
        sprintf(message+strlen(message),"Content-Type: application/json\r\n");  
        strcat(json, "{");
        for(int i=5; i<argc ;i++){                          /* body JSON      */
                
                char * token = strtok(argv[i], "=");   /* Left side     */
                strcat(json, "\"");
                strcat(json, token);
                strcat(json, "\"");
                
                strcat(json, ":");
                
                token = strtok(NULL, "=");             /* Right side     */

                if(strchr(token,';') == NULL)          /* if Right side is not list */
                {
                    strcat(json, "\"");
                    strcat(json, token);
                    strcat(json, "\"");
                }
                else                                    /* Build List */
                {
                    strcat(json, "[");

                    token = strtok(token, ";");
                    //printf("%s",token);
                    while (token!=NULL)
                    {   
                        int i =0;
                        while(token[i++]==' ') token++;
                        strcat(json, "\"");
                        strcat(json, token);
                        strcat(json, "\"");
                        token = strtok(NULL, ";");
                        if(token!=NULL)   strcat(json, ",");
                    }
                    
                    strcat(json, "]");

                }    
                if(i!=argc-1)   strcat(json, ",");
        }
        strcat(json, "}");
        sprintf(message+strlen(message),"Content-Length: %ld\r\n", strlen(json)); 
        strcat(message,"\r\n");                                  /* blank line     */
        strcat(message,json);                            
    }
        
    strcat(message, "\r\n");
    return message;
}

void prettyPrint(char *response){
    char * token = strtok(response, "[");
    char *body;

    printf("HEADER:\n\n%s\n", token);
    
    token=strtok(NULL, "[");
    
    if(token==NULL)return;

    //Body
    printf("BODY:\n");
    while(token!=NULL) {
        strcat(body,"[" );
        strcat(body,token );     
        token=strtok(NULL, "[");
    }                       

    if(body!=NULL) printBody(body);


}
void printBody(char *message){
    

    char * token = strtok(message, ",");
    char *space_two = "  ";
    char *space_for = "    ";
    char list[MAX];
    while(token!=NULL){
        
        if(strchr(token,'[') != NULL && strstr(token,"[{") == NULL ){
            memset(list, 0, sizeof(list));
            
            for(;;) {
                strcat(list, token );  
                if(strchr(token,']') != NULL) break;
                strcat(list, ", " );
                token=strtok(NULL, ",");
            }
            token = list;

       }
       if(strchr(token,'{') == NULL)  printf("\n%s%s\n",space_two, token);
       else printf("\n%s\n",token);
        token = strtok(NULL, ",");
        
    }


}
