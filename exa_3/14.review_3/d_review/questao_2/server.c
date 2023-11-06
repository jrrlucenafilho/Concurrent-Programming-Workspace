#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8600

void *connection_interface(void *arg) {
  int socket = *((int *)arg);

  while (1) {
    char buffer[13];
    char reversed_string[10];
    ssize_t valread = read(socket, buffer, 12);
    buffer[valread - 1] = '\0';

    if (strcmp("BYEBYE", buffer) == 0) {
      close(socket);
    } else if (strcmp("SHUTDOWN", buffer) == 0) {
      exit(EXIT_SUCCESS);
    }

    for (int i = 0; i < strlen(buffer); i++) {
      reversed_string[i] = buffer[strlen(buffer) - 1 - i];
    }
    reversed_string[strlen(buffer)] = '\0';

    if (strcmp(reversed_string, buffer) == 0) {
      char *response = "PALINDROMO=TRUE\n";

      send(socket, response, strlen(response), 0);
    } else {
      char *response = "PALINDROMO=FALSE\n";

      send(socket, response, strlen(response), 0);
    }
    fflush(stdout);
  }
}

int main(int argc, char *argv[]) {
  int server_fd, new_socket;
  int opt = 1;
  struct sockaddr_in address;
  socklen_t addrlen = sizeof(address);
  char buffer[1024] = {0};
  char *hello_message = "BEMVINDO\n";

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
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

  if (listen(server_fd, 6) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  while (1) {
    pthread_t tid;
    new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);

    if (new_socket < 0) {
      perror("accept");
      exit(EXIT_FAILURE);
    }

    printf("Nova conexao de %s:%d\n", inet_ntoa(address.sin_addr),
           ntohs(address.sin_port));

    send(new_socket, hello_message, strlen(hello_message), 0);

    pthread_create(&tid, NULL, connection_interface, (void *)&new_socket);
  }

  return 0;
}