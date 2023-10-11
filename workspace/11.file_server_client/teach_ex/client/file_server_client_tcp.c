#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

int main(int argc, char** argv) {
	int client_socket;
	struct sockaddr_in server_address;
	int server_port;
	char server_addr_str[1024];
	char file_name_str[256];
	char buffer_recv[1024];
	char buffer_send[1024];

	if (argc < 4) {
		printf("use: %s <ENDERECO> <PORTA> <FILE NAME>\n", argv[0]);
		exit(1);
	} else {
		server_port = atoi(argv[2]);
		strcpy(server_addr_str, argv[1]);
		strcpy(file_name_str, argv[3]);
	}

	printf("Conectando a %s:%d...\n", server_addr_str, server_port);

	client_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (client_socket == -1) {
		perror("socket() error!");
		exit(1);
	}

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(server_port);
	server_address.sin_addr.s_addr = inet_addr(server_addr_str);

	int res_connect = connect(client_socket, (struct sockaddr*)&server_address,
						sizeof(server_address));

	if (res_connect == -1) {
		perror("connect() error!");
		exit(1);
	}

	printf("Connected to %s\n", server_addr_str);

	recv(client_socket, buffer_recv, sizeof(buffer_recv), 0);
	printf("Recebi: %s\n", buffer_recv);

	printf("Solicitando arquivo: %s\n", file_name_str);

	send(client_socket, file_name_str, sizeof(file_name_str), 0);

	FILE* file_recv = fopen(file_name_str, "w");

	if (file_recv == NULL) {
		perror("Error creating file!");
		close(client_socket);
		exit(1);
	}

	char buffer_file[1024];
	int bytes_received;

	while ((bytes_received =
		recv(client_socket, buffer_file, sizeof(buffer_file),0) > 0)) {
		fputs(buffer_file, file_recv);
		printf("%s", buffer_file);
	}

	fclose(file_recv);
	close(client_socket);

	printf("Arquivo %s recebido com sucesso!\n", file_name_str);

	return 0;

}
