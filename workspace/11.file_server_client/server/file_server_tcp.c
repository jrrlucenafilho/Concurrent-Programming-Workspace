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
#define SERVER_PORT 9090

struct client_info {
	int socket;
	struct sockaddr_in address;
};

void* client_handle(void* p) {
	struct client_info* c_info = (struct client_info*) p;

    char buffer_send[1024];
    char buffer_recv[1024];
	char client_address_str[INET_ADDRSTRLEN];

    FILE* file_send;

	inet_ntop(AF_INET, &(c_info->address.sin_addr), client_address_str, INET_ADDRSTRLEN);

	printf("New connection from: %s:%d\n", client_address_str, ntohs(c_info->address.sin_port));

    strcpy(buffer_send, "WELCOME\n");
    send(c_info->socket, buffer_send, strlen(buffer_send), 0);
    recv(c_info->socket, buffer_recv, sizeof(buffer_recv), 0);

	while(true){
        strcpy(buffer_send, "WELCOME\n");
        send(c_info->socket, buffer_send, strlen(buffer_send), 0);

        recv(c_info->socket, buffer_recv, sizeof(buffer_recv), 0);
        printf("Client asking for file: %s\n", buffer_recv);

        buffer_recv[strlen(buffer_recv)] = '\0';

        file_send = fopen(buffer_recv, "r");

        if(file_send == NULL){
            perror("File not found\n");
            strcpy(buffer_send, "ERROR: FILE NOT FOUND\n");
            send(c_info->socket, buffer_send, strlen(buffer_send), 0);
            close(c_info->socket);
            return 0;
        }

        char buffer_file[1024];
        size_t bytes_read;

		//Matters if file has more chars than 1024 (sends to client for every 1024-bytes string)
        while((bytes_read = fread(buffer_file, 1, sizeof(buffer_file), file_send)) > 0){
            send(c_info->socket, buffer_file, bytes_read, 0);
        }

        printf("File sent: %s\n", buffer_file);

        fclose(file_send);
        close(c_info->socket);
	}

}

//Same thing here
int main(void)
{
	int server_socket;
	struct sockaddr_in server_addr;

	struct client_info client_data[MAX_CLIENTS];
	pthread_t client_threads[MAX_CLIENTS];

	int client_counter = 0;

	server_socket = socket(AF_INET, SOCK_STREAM, 0);

	if(server_socket == -1){
		perror("socket() error!");
		exit(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	int bind_res = bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

	if(bind_res == -1){
		perror("bind() error!!");
		exit(1);
	}

    //Associate buffer to server socket
	int listen_res = listen(server_socket, MAX_CLIENTS);

	if(listen_res == -1){
		perror("listen() error!!");
		exit(1);
	}

	printf("Waiting for new connections on port %d...\n", SERVER_PORT);

	while(true){
		struct sockaddr_in client_address;
		socklen_t client_address_len = sizeof(client_address);

	   	int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);

		if (client_socket == -1) {
			perror("accept() error!");
			continue;
		}

		printf("Received a new connection\n");

		struct client_info* c_info = (struct client_info*)malloc(sizeof(struct client_info));

		c_info->socket = client_socket;
		c_info->address = client_address;

		pthread_create(&client_threads[client_counter], 0, client_handle, (void*) c_info);

		client_counter = ((client_counter + 1) % MAX_CLIENTS);
	}

	close(server_socket);


	return 0;
}

