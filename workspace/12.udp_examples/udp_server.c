#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_PORT 9990
#define BUFFER_SIZE 1024

int main(void) {
	int server_fd;
	struct sockaddr_in server_addr;
	char buffer[BUFFER_SIZE];

	server_fd = socket(AF_INET, SOCK_DGRAM, 0);

	if (server_fd == -1) {
		perror("socket() error!");
		exit(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	int bind_res = bind(server_fd,
			    (struct sockaddr*) &server_addr,
			     sizeof(server_addr));

	if (bind_res == -1) {
		perror("bind() error!");
		exit(1);
	}

	while (1) {
		struct sockaddr_in client_addr;
		unsigned int cli_addr_len;
		char* ret_msg = "RECV OK!\n";


		printf("Aguardando mensagens UDP na porta: %d\n", SERVER_PORT);
		int recv_len = recvfrom(server_fd, buffer, sizeof(buffer), 0,
					(struct sockaddr*)&client_addr,
					 &cli_addr_len);

		buffer[recv_len] = '\0';

		printf("Recebi msg de: (%s:%d) : %s\n",
			inet_ntoa(client_addr.sin_addr),
		        ntohs(client_addr.sin_port), buffer);

		sendto(server_fd, ret_msg, strlen(ret_msg), 0,
			(struct sockaddr*) &client_addr, cli_addr_len);

	}

	close(server_fd);

	return 0;

}
