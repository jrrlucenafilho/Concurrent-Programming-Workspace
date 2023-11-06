#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8600

int server_fd;

typedef struct connection_interface_t {
  char *msg;
  struct sockaddr_in client_addr;
  unsigned int cli_addr_len;
} connection_interface_t;

void *connection_interface(void *arg) {
  connection_interface_t cit = *((connection_interface_t *)arg);
  char *msg = cit.msg;
  struct sockaddr_in client_addr;
  unsigned int cli_addr_len;

  client_addr = cit.client_addr;
  cli_addr_len = cit.cli_addr_len;

  printf("%s\n", msg);

  char reversed_string[10];

  if (strcmp("SHUTDOWN", msg) == 0) {
    exit(EXIT_SUCCESS);
  }

  for (int i = 0; i < strlen(msg); i++) {
    reversed_string[i] = msg[strlen(msg) - 1 - i];
  }
  reversed_string[strlen(msg)] = '\0';

  if (strcmp(reversed_string, msg) == 0) {
    char *response = "PALINDROMO=TRUE\n";

    sendto(server_fd, response, strlen(response), 0,
           (struct sockaddr *)&client_addr, cli_addr_len);
  } else {
    char *response = "PALINDROMO=FALSE\n";

    sendto(server_fd, response, strlen(response), 0,
           (struct sockaddr *)&client_addr, cli_addr_len);
  }
}

int main(int argc, char *argv[]) {
  int opt = 1;
  struct sockaddr_in address;
  socklen_t addrlen = sizeof(address);
  char buffer[1024] = {0};
  char *hello_message = "BEMVINDO\n";

  if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Failed to create socket");
    exit(EXIT_FAILURE);
  }

  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  while (1) {
    connection_interface_t cit;
    struct sockaddr_in client_addr;
    unsigned int cli_addr_len;
    pthread_t tid;
    int recv_len = recvfrom(server_fd, buffer, sizeof(buffer), 0,
                            (struct sockaddr *)&client_addr, &cli_addr_len);

    buffer[recv_len - 1] = '\0';

    cit.msg = buffer;
    cit.client_addr = client_addr;
    cit.cli_addr_len = cli_addr_len;

    printf("Nova conexao de %s:%d\n", inet_ntoa(client_addr.sin_addr),
           ntohs(client_addr.sin_port));

    sendto(server_fd, hello_message, strlen(hello_message), 0,
           (struct sockaddr *)&client_addr, cli_addr_len);

    pthread_create(&tid, NULL, connection_interface, (void *)&cit);
  }

  return 0;
}