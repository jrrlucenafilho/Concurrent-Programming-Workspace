#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9990
#define BUFFER_SIZE 1024

int main(void) {
	int client_fd;

        struct sockaddr_in server_addr;

        char buffer[BUFFER_SIZE];

        client_fd = socket(AF_INET, SOCK_DGRAM, 0);

        if (client_fd == -1) {
                perror("socket() error!");
                exit(1);
        }

        server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

	while(1) {
		printf("Digite uma mensagem: ");
		fgets(buffer, sizeof(buffer), stdin);

		buffer[strlen(buffer)-2] - '\0'; //ATS: retirando o \n

		printf("Enviando: %s\n", buffer);

		size_t len = sendto(client_fd, buffer, strlen(buffer), 0,
				    (struct sockaddr*) &server_addr,
				     sizeof(server_addr));

		if (len == -1) {
			perror("sendto() error!");
			exit(1);
		}

		len = recvfrom(client_fd, buffer, sizeof(buffer), 0,
			       NULL, NULL);
		buffer[len] = '\0';

		printf("Recebi do servidor: %s\n", buffer);

	}

	close(client_fd);

	return 0;
}



