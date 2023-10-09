#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define SERVER_PORT 9091
#define SERVER_ADDR "127.0.0.1"

int main(void) {
	int client_socket;
	struct sockaddr_in server_address;
	char buffer_recv[1024];
	char buffer_send[1024];

	client_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (client_socket == -1) {
		perror("socket() error!");
		exit(1);
	}

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(SERVER_PORT);
	server_address.sin_addr.s_addr = inet_addr(SERVER_ADDR);

	int res_connect = connect(client_socket, (struct sockaddr*)&server_address,
						sizeof(server_address));

	if (res_connect == -1) {
		perror("connect() error!");
		exit(1);
	}

	printf("Connected to %s\n", SERVER_ADDR);

	while (1) {

		recv(client_socket, buffer_recv, sizeof(buffer_recv), 0);
		printf("Recebi do servidor: %s\n", buffer_recv);

		printf("Digite uma mensagem para enviar: ");

		scanf("%s", buffer_send);

		send(client_socket, buffer_send, sizeof(buffer_send), 0);

		printf("Enviei: %s\n", buffer_send);

		if (buffer_send[0] == 'e' || buffer_send[0] == 'E') {
			printf("Fechando a conexao com servidor!\n");
			close(client_socket);
			exit(0);
		}
	}
}
