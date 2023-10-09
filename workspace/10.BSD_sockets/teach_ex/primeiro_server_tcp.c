#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_CLIENTS 10
#define SERVER_PORT 9090


int main(void) {
	int server_socket;
	int client_sockets[MAX_CLIENTS];
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addrs[MAX_CLIENTS];
	int client_counter = 0;


	server_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (server_socket == -1) {
		perror("socket() error!");
		exit(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	int bind_res = bind(server_socket, (struct sockaddr*)&server_addr,
						sizeof(server_addr));

	if (bind_res == -1) {
		perror("bind() error!!");
		exit(1);
	}

	int listen_res = listen(server_socket, MAX_CLIENTS);

	if (listen_res == -1) {
		perror("listen() error!!");
		exit(1);
	}

	printf("Waiting for new connections...\n");

	while(1) {

	   	client_sockets[client_counter] = accept(server_socket, NULL, NULL);

//	client_sockets[client_counter] =
//			accept(server_socket,
//				(struct sockaddr*)&client_addrs[client_counter],
//				(int) sizeof(client_addrs[client_counter]));

		char buffer_send[1024];
		char buffer_recv[1024];

		printf("Received a new connection!\n");

//	recv(client_sockets[client_counter], buffer, sizeof(buffer), 0);

//	printf("Mensagem do cliente: %s\n", buffer);

		strcpy(buffer_send, "MENSAGEM DE BOAS VINDAS AO CLIENTE\n");

		send(client_sockets[client_counter], buffer_send, strlen(buffer_send), 0);

		sleep(1);

	        recv(client_sockets[client_counter], buffer_recv, sizeof(buffer_recv), 0);

	        printf("Mensagem do cliente: %s\n", buffer_recv);


		close(client_sockets[client_counter]);
		client_counter++;

	} //while

	close(server_socket);


	return 0;
}

