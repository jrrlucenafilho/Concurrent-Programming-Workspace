#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define PORT 8600

void *connection_interface(void *arg) {
  int socket = *((int *)arg);
  char buffer[1024];

  while (1) {
    ssize_t valread = read(socket, buffer, 1024 - 1);

    if (strstr(buffer, "BCKWRD") != NULL) {
      char *arg = strtok(buffer, " ");
      arg = strtok(NULL, " ");
      arg[strlen(arg) - 1] = '\0';

      char response[strlen(arg) + 2];

      for (int i = 0; i < strlen(arg); i++) {
        response[i] = arg[strlen(arg) - 1 - i];
      }
      strcat(response, "\n\0");

      send(socket, response, strlen(response), 0);
    } else if (strstr(buffer, "RNDNUM") != NULL) {
      int gen_number = rand() % 1001;
      char response[7];

      sprintf(response, "%d", gen_number);
      strcat(response, "\n\0");

      send(socket, response, strlen(response), 0);
    } else if (strstr(buffer, "FINISH") != NULL) {
      close(socket);
    } else {
      const char *response = "ERROR\n";

      send(socket, response, strlen(response), 0);
    }
  }

  return NULL;
}

int main(int argc, char *argv[]) {
  srand(time(NULL));

  int server_fd, new_socket;
  int opt = 1;
  ssize_t valread;
  struct sockaddr_in address;
  socklen_t addrlen = sizeof(address);
  char buffer[1024] = {0};
  char *hello_message = "WELCOME\n";

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Failed creating socket");
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

  if (listen(server_fd, 5) < 0) {
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

    send(new_socket, hello_message, strlen(hello_message), 0);

    pthread_create(&tid, NULL, connection_interface, (void *)&new_socket);
  }

  close(server_fd);

  return 0;
}