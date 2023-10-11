#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {
	int client_socket;
    int server_port;
	struct sockaddr_in server_address;
    char server_addr_str[1024];
    char file_name_str[1024];
	char buffer_recv[1024];
	char buffer_send[1024];

    if(argc < 4){
        printf("Wrong args bud\n");
        exit(1);
    }else{
        server_port = atoi(argv[2]);
        strcpy(server_addr_str, argv[1]);
    }

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

	while(true){

		recv(client_socket, buffer_recv, sizeof(buffer_recv), 0);
		printf("Recebi do servidor: %s\n", buffer_recv);

		printf("Solicitando arquivo: %s\n", file_name_str);

		scanf("%s", buffer_send);

		send(client_socket, file_name_str, sizeof(file_name_str), 0);

		printf("Enviei: %s\n", buffer_send);

        FILE* file_recv = fopen(file_name_str, "w");

        if(file_recv == NULL){
            perror("Error creating file!");
            close(client_socket);
            exit(1);
        }

		char buffer_file[1024];
		int bytes_received;

		while((bytes_received = recv(client_socket, buffer_file, sizeof(buffer_file), 0) > 0)){
			fwrite(buffer_file, 1, bytes_received, file_recv);
			// fputs()
			printf("%s", buffer_file);
		}

		fclose(file_recv);
		close(client_socket);
	}
}
