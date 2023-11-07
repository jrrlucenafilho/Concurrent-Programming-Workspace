#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>

#define MAX_CLIENTS 10
#define TCP_SERVER_PORT 9600
#define BUFFER_SIZE 1024

//Client handler
void* handle_client(void* p)
{
	int client_socket = (int)p;

	char integer_prompt_to_client[BUFFER_SIZE] = "AWAITING FOR CLIENT INTEGER INPUT (UDP PORT NUMBER): ";
    char random_number_announcement_to_udp[BUFFER_SIZE] = "RANDOM NUMBER RECEIVED: ";
	char buffer_recv[BUFFER_SIZE];
	char buffer_send[BUFFER_SIZE];

    int udp_port_int;

    //Stuff regarding the udp port connection
    int udp_server_fd;
    struct sockaddr_in server_addr;
    char random_num_buffer[BUFFER_SIZE];

	//Notifies the server of connection
	printf("\nNew connection from client: %d\n", client_socket);

    while(true){
        //Tells server that's it's waiting for user input
        printf("Awaiting integer (UDP Port number) from client...\n");

        //Sends prompt to client
        send(client_socket, integer_prompt_to_client, strlen(integer_prompt_to_client), 0);
        
        //And awaits to receive the number (udp port number)
        recv(client_socket, buffer_recv, sizeof(buffer_recv), 0);
        buffer_recv[strlen(buffer_recv)] = '\0'; //cleaning '\n'

        //Notify server of receiving the number (still as a string)
        printf("\nClient %d sent: %s\n", client_socket, buffer_recv);

        //Now transforms string received to number that'll be the udp port
        udp_port_int = atoi(buffer_recv);

        //Checks if user entered a 0, and closes connection if so
        if(udp_port_int == 0){
            //Notifies server about closing connection
            printf("Client %d inserted 0, closing connection...\n", client_socket);

            //Notifies client
            strcpy(buffer_send, "0 INSERTED BY CLIENT, CLOSING CONNECTION...\n");
            send(client_socket, buffer_send, strlen(buffer_send), 0);

            //Closes connection and exits
            close(client_socket);

            return 0;
        }

        //If it got here, it means udp_port_int holds a udp port number inserted by the client
        //Send a random number to this port

        //Connects to socket
        udp_server_fd = socket(AF_INET, SOCK_DGRAM, 0);

        //Connecting failsafe
        if(udp_server_fd == -1){
            perror("socket() error!");
            exit(1);
        }

        //Populate udp server info (at this point it's guaranteed udp_port_int is an udp port number)
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(udp_port_int);
        server_addr.sin_addr.s_addr = INADDR_ANY;

        //Binds udp server
        int bind_res = bind(udp_server_fd, (struct sockaddr*) &server_addr, sizeof(server_addr));

        //Binding failsafe
        if(bind_res == -1){
            perror("bind() error!");
            exit(1);
	    }

        //Setting up UDP server vars
        struct sockaddr_in client_addr;
		unsigned int cli_addr_len = sizeof(client_addr);

        //To receive the random number to the terminal:
        //First send the random number to and udp port through the tcp server.
        //Then:
        //nc -u localhost <udp port sent to tcp server> and press enter enter
        //It prints: RANDOM NUMBER RECEIVED: <random number>

        //Now to actually send the random number to the udp server, since it's been bound already
        //Making the random number
        srand(time(NULL));  //Seeding with current time
        int random_number = rand();

        //Converting random number to string
        sprintf(random_num_buffer, "%d", random_number);

        //Notifying original TCP server that it's about to send it
        printf("Sending random number to UDP server in port %d...\n", udp_port_int);

        //To establish connection
        recvfrom(udp_server_fd, buffer_recv, sizeof(buffer_recv), 0, (struct sockaddr*)&client_addr, &cli_addr_len);

        //First sending the random number announcement to the udp server
        sendto(udp_server_fd, random_number_announcement_to_udp, strlen(random_number_announcement_to_udp), 0, (struct sockaddr*) &client_addr, cli_addr_len);

        //Sending the random number to selected udp port
        sendto(udp_server_fd, random_num_buffer, strlen(random_num_buffer), 0, (struct sockaddr*) &client_addr, cli_addr_len);

        //And then closing the UDP connection
        close(udp_server_fd);
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