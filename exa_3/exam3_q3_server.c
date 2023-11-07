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
#define TCP_SERVER_PORT 9601
#define BUFFER_SIZE 1024

//Will hold both server and client values
//Done this way so it can close the client AND the SERVER if the client sends 0 (with the flag)
typedef struct {
    int server_socket;
    int client_socket;
    bool close_server_flag;
} socket_pair_t;

//Client handler
void* handle_client(void* p)
{
	socket_pair_t* socket_pair = (socket_pair_t*)p;

	char integer_prompt_to_client[BUFFER_SIZE] = "AWAITING FOR CLIENT INTEGER INPUT (UDP PORT NUMBER): ";
    char random_number_announcement_to_udp[BUFFER_SIZE] = "RANDOM NUMBER RECEIVED: ";
	char buffer_recv[BUFFER_SIZE];
	char buffer_send[BUFFER_SIZE];

    int udp_port_int;

    //Vars regarding the udp port connection
    int udp_server_fd;
    struct sockaddr_in server_addr;
    char random_num_buffer[BUFFER_SIZE];

	//Notifies the server of connection
	printf("\nNew connection from client: %d\n", socket_pair->client_socket);

    while(true){
        //Tells server that's it's waiting for user input
        printf("Awaiting integer (UDP Port number) from client...\n");

        //Sends prompt to client
        send(socket_pair->client_socket, integer_prompt_to_client, strlen(integer_prompt_to_client), 0);
        
        //And awaits to receive the number (udp port number)
        recv(socket_pair->client_socket, buffer_recv, sizeof(buffer_recv), 0);
        buffer_recv[strlen(buffer_recv)] = '\0'; //cleaning '\n'

        //Notify server of receiving the number (still as a string)
        printf("\nClient %d sent: %s\n", socket_pair->client_socket, buffer_recv);

        //Now transforms string received to number that'll be the udp port
        udp_port_int = atoi(buffer_recv);

        //Checks if user entered a 0, and closes connection if so
        if(udp_port_int == 0){
            //Notifies server about closing connection
            printf("Client %d inserted 0, closing connection...\n", socket_pair->client_socket);

            //Notifies client
            strcpy(buffer_send, "0 INSERTED BY CLIENT, CLOSING CONNECTION...\n");
            send(socket_pair->client_socket, buffer_send, strlen(buffer_send), 0);

            //Sets the close SERVER flag to true to finalize the server, as asked by the specification
            socket_pair->close_server_flag = true;

            //Closes connection with client
            close(socket_pair->client_socket);

            exit(0);
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
        //Testado e Funcionando:
        //First send the random number to an udp port through the tcp server.
        //Then:
        //nc -u localhost <udp port sent to tcp server> and press enter enter
        //It prints: RANDOM NUMBER RECEIVED: <random number> on the udp client's side

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

        //Concatenating random number string to end of announcement string
        strcat(random_number_announcement_to_udp, random_num_buffer);

        //Sending the announcement + random number to selected udp port
        sendto(udp_server_fd, random_number_announcement_to_udp, strlen(random_number_announcement_to_udp), 0, (struct sockaddr*) &client_addr, cli_addr_len);

        //Removing random_num_buffer from random_number_announcement_to_udp to clear the number for next iteration
        //(so it doesn't keep just appending the number over and over ad-hoc)
        char* substring_position = strstr(random_number_announcement_to_udp, random_num_buffer);
        size_t length_to_remove = strlen(random_num_buffer);
        memmove(substring_position, substring_position + length_to_remove, strlen(substring_position + length_to_remove) + 1);

        //And then closing the UDP connection
        close(udp_server_fd);
    }

	return 0;
}

int main(void)
{
    socket_pair_t* socket_pair = (socket_pair_t *)malloc(sizeof(socket_pair_t));
    struct sockaddr_in server_addr;
    pthread_t client_threads[MAX_CLIENTS];
    int client_counter = 0;

    socket_pair->server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if(socket_pair->server_socket == -1){
        perror("socket() error!");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(TCP_SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int bind_res = bind(socket_pair->server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

	if(bind_res == -1){
		perror("bind() error!");
		exit(1);
	}

	int listen_res = listen(socket_pair->server_socket, MAX_CLIENTS);

	if(listen_res == -1){
		perror("listen() error!");
		exit(1);
	}

    printf("Waiting for new connections on port %d...\n", TCP_SERVER_PORT);

    while(true){
		socket_pair->client_socket = accept(socket_pair->server_socket, NULL, NULL);

		pthread_create(&client_threads[client_counter], NULL, handle_client, (void*)socket_pair);

		client_counter = ((client_counter + 1) % MAX_CLIENTS);

        //Checks if the client inserted a '0'. Which means the server must be finalized 
        if(socket_pair->close_server_flag){
            printf("Server is shutting down as requested by client %d...\n", socket_pair->client_socket);
            close(socket_pair->server_socket);
            free(socket_pair);
            exit(0);
        }
	}

	close(socket_pair->server_socket);

	return 0;
}