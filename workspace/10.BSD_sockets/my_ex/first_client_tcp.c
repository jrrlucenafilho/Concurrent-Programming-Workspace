#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>

#define SERVER_PORT 9090
#define SERVER_ADDR "127.0.0.1"

//Clients should only invoke connect read and write
//socket() -> accept() -> connect() -> read() -> write()
//Connecting it to the server program!
//Can be looped (along with server) for 

int main(void)
{
    int client_socket;
    struct sockaddr_in server_address;

    char buffer_recv[1024];
    char buffer_send[1024];

    //Creating client socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    if(client_socket == -1){
        perror("socket() error!");
        exit(1);
    }

    //Populating struct (server_address)

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    
    //now HERE it's diff from server (have to choose ip of server this client'll be connecting with)
    server_address.sin_addr.s_addr = inet_addr(SERVER_ADDR);

    int res_connect = connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));

    if(res_connect == -1){
        perror("connect() error!");
        exit(1);
    }

    //Here we don't really need a bind(). Since server info is already passed through connect()
    recv(client_socket, buffer_recv, sizeof(buffer_recv), 0);
    printf("Recebi do servidor: %s\n", buffer_recv);

    printf("Digite a uma mensagem para enviar: ");
    scanf("%s", buffer_send);
    send(client_socket, buffer_send, sizeof(buffer_send), 0);

    close(client_socket);
}