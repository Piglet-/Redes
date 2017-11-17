/*
    C ECHO client example using sockets
*/
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<unistd.h>
#include<stdlib.h>
#include <time.h>
 
int main(int argc , char *argv[])
{

	if (argc != 7) {
        fprintf(stderr,"Uso: %s -d <nombre_modulo_central> -p <puerto_svr_s> -l <puerto_local>\n", argv[0]);
        exit(1);
    }

    int sock, bytes_recv;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];

    char *hostname;
    char *flag_hostname = strdup("-d");
    char *flag_localport = strdup("-l");
    char *flag_serverport = strdup("-p");
    FILE *fp;
    long localport;
    long serverport;

    int n = 1;
    int k = 2;
    int i;

    for(i = 0 ; i < 3 ; i++){
        if (strcmp(argv[n],flag_hostname) == 0){
          hostname = strdup(argv[k]);
        } else if (strcmp(argv[n],flag_localport) == 0){
          localport = strtol(argv[k],0,10);
        } else if (strcmp(argv[n],flag_serverport) == 0){
          serverport = strtol(argv[k],0,10);
        } else {
          printf("ERROR: Argumento %s no reconocido.\n", argv[n]);
          exit(1);
        }
        n = n + 2;
        k = k + 2;
    }


    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
     
    puts("Connected\n");
   
    fp = fopen("reg.txt" , "rb+");

    if (fp == NULL){

        fp = fopen("reg.txt", "wb");
        fclose(fp);
    
        while(1)
        {
            printf("Enter message : ");
            scanf ("%[^\n]%*c", message);
            

            time_t rawtime;
            struct tm * timeinfo;
            

            char output[2000];
            

            time ( &rawtime );
            timeinfo = localtime ( &rawtime );
            

            sprintf(output,"[%d:%d:%d %d-%d-%d] %s",timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec,timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, message);
            
            printf("Sending message %s\n", output);
            
            //Send some data
            if( send(sock , output , strlen(output) , 0) < 0)
            {
                puts("Send failed");
                return 1;
            }
            
            //Receive a reply from the server
            bytes_recv = recv(sock , server_reply , 2000 - 1 , 0);
            server_reply[bytes_recv] = '\0';
            if( bytes_recv < 0)
            {
                puts("recv failed");
                break;
            }
            puts("Server reply :");
            puts(server_reply);

        }
    } else {

        //Receive a reply from the server
            bytes_recv = recv(sock , server_reply , 2000 - 1 , 0);
            server_reply[bytes_recv] = '\0';
            if( bytes_recv < 0)
            {
                puts("recv failed");
                return 0;
            }
            puts("Server reply :");
            puts(server_reply);
            
             while(1)
        {
            printf("Enter message : ");
            scanf ("%[^\n]%*c", message);
            

            time_t rawtime;
            struct tm * timeinfo;
            

            char output[2000];
            

            time ( &rawtime );
            timeinfo = localtime ( &rawtime );
            

            sprintf(output,"[%d:%d:%d %d-%d-%d] %s",timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec,timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, message);
            
            printf("Sending message %s\n", output);
            
            //Send some data
            if( send(sock , output , strlen(output) , 0) < 0)
            {
                puts("Send failed");
                return 1;
            }
            
            //Receive a reply from the server
            bytes_recv = recv(sock , server_reply , 2000 - 1 , 0);
            server_reply[bytes_recv] = '\0';
            if( bytes_recv < 0)
            {
                puts("recv failed");
                break;
            }
            puts("Server reply :");
            puts(server_reply);

        }
     
    }
     
    close(sock);
    return 0;
}



