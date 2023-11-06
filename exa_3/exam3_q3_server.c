#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX_CLIENTS 10
#define TCP_SERVER_PORT 9600
#define BUFFER_SIZE 1024

//CLient handler
void* handle_client(void* p)
{
	int client_socket = (int)p;

	char first_prompt_buffer[BUFFER_SIZE] = "AWAITING FOR CLIENT INTEGER INPUT (UDP PORT NUMBER): ";
	char buffer_recv[BUFFER_SIZE];
	char buffer_send[BUFFER_SIZE];

    int udp_port_int;


	//Notifies the server of connection
	printf("New connection from client: %d\n", client_socket);

    while(true){
        //Tells server that's it's waiting for user input
        printf("Awaiting integer (UDP Port number) from client...\n");

        //Sends prompt to client
        send(client_socket, first_prompt_buffer, strlen(first_prompt_buffer), 0);
        
        //And awaits to receive the number (udp prort)
        recv(client_socket, buffer_recv, sizeof(buffer_recv), 0);
        buffer_recv[strlen(buffer_recv)] = '\0'; //cleaning CR

        //Notify server of receiving the number (still as a string)
        printf("Client %d sent: %s\n", client_socket, buffer_recv);

        //Now transforms string received to number that'll be the udp port
        udp_port_int = atoi(buffer_recv);

        //Checks if user entered a 0, and closes connection if so
        if(udp_port_int == 0){
            //Notifies server about closing connection
            printf("Client %d inserted 0, closing connection...\n", client_socket);

            //Notifies client
            strcpy(buffer_send, "0 INSERTED, CLOSING CONNECTION...\n");
            send(client_socket, buffer_send, strlen(buffer_send), 0);

            //Closes connection and exits
            close(client_socket);

            return 0;
        }

        //If it got here, it means udp_port_int holds a udp port inserted by the client
        //Send a random number to this port

    }

	return 0;
}

int main(void)
{
    int server_socket;
    struct sockaddr_in server_addr;
    pthread_t client_threads[MAX_CLIENTS];
    int client_counter = 0;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if(server_socket == -1){
        perror("socket() error!");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(TCP_SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int bind_res = bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

	if(bind_res == -1){
		perror("bind() error!");
		exit(1);
	}

	int listen_res = listen(server_socket, MAX_CLIENTS);

	if(listen_res == -1){
		perror("listen() error!");
		exit(1);
	}

    printf("Waiting for new connections on port %d...\n", TCP_SERVER_PORT);

    while(true){
		int client_socket = accept(server_socket, NULL, NULL);

		pthread_create(&client_threads[client_counter], NULL, handle_client, (void*)client_socket);

		client_counter = ((client_counter + 1) % MAX_CLIENTS);
	}

	close(server_socket);

	return 0;
}