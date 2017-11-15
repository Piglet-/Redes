/*
    C socket server example, handles multiple clients using threads
*/
 
#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread
#include <string.h>

char *file;
//the thread function
void *connection_handler(void *);
 
int main(int argc , char *argv[])
{
    if (argc != 5) {
        fprintf(stderr,"Uso: %s -l <puerto_srv_c> -b <archivo_bitácora>\n", argv[0]);
        exit(1);
    }

    char *flag_port;
    char *flag_log_file;
    file = argv[4];
    flag_port = strdup("-l");
    flag_log_file = strdup("-b");

    int n, k;
    long port;
    char *log_file_name;

    n = 1;
    k = 2;

    int i;

    for(i = 0 ; i < 2 ; i++){
        if (strcmp(argv[n],flag_port) == 0){
          port = strtol(argv[k],0,10);
        } else if (strcmp(argv[n],flag_log_file) == 0){
          log_file_name = strdup(argv[k]);
        } else {
          printf("ERROR: Argumento %s no reconocido.\n", argv[n]);
          exit(1);
        }
        n = n + 2;
        k = k + 2;
    }

    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
     
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");
         
        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;
         
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }
         
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        puts("Handler assigned");
    }
     
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
     
    return 0;
}
 
/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char *message , client_message[2000];
    char * pch;
    char * str;
    FILE *fp; 

    

    //Send some messages to the client
    message = "Greetings! I am your connection handler \n";
    //write(sock , message , strlen(message));
     
    message = "Now type something and i shall repeat what you type \n";
    //write(sock , message , strlen(message));

     
    //Receive a message from client
    while( (read_size = recv(sock , client_message , 2000 - 1 , 0)) > 0 )
    {
        //Send the message back to client
        client_message[read_size] = '\0';
        write(sock , client_message , strlen(client_message));
        
        fp = fopen( file , "a" );

        pch = strtok (client_message," :");
        if (pch != NULL)
        {
            fwrite("(" , 1 , sizeof(char) , fp );
            fwrite(pch , 1 , strlen(pch) , fp );
            fwrite(", " , 1 , sizeof(char) * 2 , fp );
            pch = strtok (NULL, " :");
            
            fwrite(pch , 1 , strlen(pch) , fp );
            fwrite(":" , 1 , sizeof(char) , fp );
            pch = strtok (NULL, " :");
            
            fwrite(pch , 1 , strlen(pch) , fp );
            fwrite(", " , 1 , sizeof(char) * 2 , fp );
            pch = strtok (NULL, " :");

            fwrite(pch , 1 , strlen(pch) , fp );
            fwrite(":" , 1 , sizeof(char) , fp );
            pch = strtok (NULL, " :");

            fwrite(pch , 1 , strlen(pch) , fp );
            fwrite(", " , 1 , sizeof(char) * 2 , fp );
            pch = strtok (NULL, " :");

            fwrite(pch , 1 , strlen(pch) , fp );
            fwrite(", " , 1 , sizeof(char) * 2 , fp );
            pch = strtok (NULL, " :");

        }

        while (pch != NULL)
        {
            fwrite(pch , 1 , strlen(pch) , fp );
            fwrite(" " , 1 , sizeof(char) , fp );
            pch = strtok (NULL, " :");

        }

    fclose(fp);
    }
     
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
         
    //Free the socket pointer
    free(socket_desc);
     
    return 0;
}
