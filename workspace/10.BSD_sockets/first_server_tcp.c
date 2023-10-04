#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

//Create a file descriptor first
//Teach said this API is a bit ugly but eh,there's enough docs around
//Socket: ignore third param
//Sock_stream: TCP
//Dgram: UDP

#define MAX_CLIENTS 10
#define SERVER_PORT 9090

int main(void)
{
    int server_socket;
    int client_sockets[MAX_CLIENTS];
    int client_counter = 0;

    struct sockaddr_in server_addr; //Holds server info

    struct sockaddr_in client_addr; //Holds client info
    struct sockaddr_in client_addrs[MAX_CLIENTS];

    //init socket (we'll always be using AF_NET here)
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if(server_socket == -1){
        //faile to create socket
        perror("socket() error\n");
        exit(1);
    }

    //POPULATING STRUCT
    //now, bind struct to socket, telling the struct (again) that'll use  AF_INET
    server_addr.sin_family = AF_INET;

    //telling it which port to use, harmonizing (formatting) according to OS standart for bits
    server_addr.sin_port = htons(SERVER_PORT);

    //Signal to the OS that i'm a server, not connecting to anyone, but ppl will conect to me
    server_addr.sin_addr.s_addr = INADDR_ANY;

    //Sync this struct to my socket, "binding" it with bind() (yep)
    //casting from sockaddr_in to sockaddr*
    int bind_res = bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    //Checking for errors
    if(bind_res == -1){
            //failed to bind struct to socket
        perror("bind() error\n");
        exit(1);
    }

    //Telling the OS my backlog size (MAX_CLIENTS here)
    int listen_res = listen(server_socket, MAX_CLIENTS);

    if(listen_res == -1){
            //failed to bind struct to socket
        perror("listen() error\n");
        exit(1);
    }

    //Now we're ready to wait for connections
    printf("Awaiting connections...\n");

    //accept'll give me info about the clients who connect
    //only accept4() receives flags
    //client_sockets[client_counter] = accept(server_socket,
    //                                        (struct sockaddr*)&client_addrs[client_counter],
    //                                        (int)sizeof(client_addrs[client_counter]));

    client_sockets[client_counter] =  accept(server_socket, NULL, NULL);

    //Now awaiting a msg from client, in a buffer
    char buffer_send[1024];
    char buffer_recv[1024];

    printf("RECEIVED A NEW CONNECTION\n");

    strcpy(buffer_send, "MENSAGEM DE BOAS VINDAS AO CLIENTE\n");

    send(client_sockets[client_counter], buffer_send, strlen(buffer_send), 0);

    recv(client_sockets[client_counter], buffer_recv, sizeof(buffer_recv), 0);

    printf("Mensagem do cliente: %s\n", buffer_recv);

    close(client_sockets[client_counter]);
    close(server_socket);   //And closes after receiving msg from client

    return 0;

    //Were i to use a while-loop, the msg would only show the welcom msg to the first client
    //So we'll have to paralelize clients, dealing with them concurrently (each thread is a client (next class))

    //We'll be creating a lil' chat service
}