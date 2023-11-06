//Servidor TCP
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX_CLIENTS 5
#define SERVER_PORT 8601
#define LOG_FILENAME "log.txt"

typedef struct {
        FILE* logfile;
        pthread_mutex_t mutex;
} t_monitor;

void inicializa_monitor(t_monitor* m) {
	pthread_mutex_init(&m->mutex, NULL);
}

void finaliza_monitor(t_monitor* m) {
	pthread_mutex_destroy(&m->mutex);
	fclose(m->logfile);
}

void log_monitor(t_monitor* m, char* msg) {
	pthread_mutex_lock(&m->mutex);
	m->logfile = fopen(LOG_FILENAME, "a");
	fprintf(m->logfile, "%s\n", msg);
	fclose(m->logfile);
	pthread_mutex_unlock(&m->mutex);
}

t_monitor _log;

void reverse_str(char* str1, char* str2) {
	int r = strlen(str1) - 1;
	for (int i = 0; i < strlen(str1); i++) {
		str2[i] = str1[r];
		r--;
	}
}

void* handle_client(void* p) {
	int client_socket = (int)p;

	char buffer_send_w[20];

	strcpy(buffer_send_w, "WELCOME!\n");
        send(client_socket, buffer_send_w, strlen(buffer_send_w), 0);

	printf("New connection from client: %d\n", client_socket);

	while(true) {
		char buffer_recv[1024];
		char buffer_send[1024];

	        recv(client_socket, buffer_recv, sizeof(buffer_recv), 0);
		buffer_recv[strlen(buffer_recv)] = '\0';

	        printf("Client: %s\n", buffer_recv);

		log_monitor(&_log, buffer_recv);

		//END ends connection
		if(strncmp("END", buffer_recv, 3) == 0){
			strcpy(buffer_send, "FINALIZING CONNECTION\n");
			send(client_socket, buffer_send, strlen(buffer_send), 0);

			close(client_socket);

			printf("Closing client: %d\n", client_socket);
			return 0;

		//ADD <INT1> <INT2>\n
		}else if(strncmp("ADD", buffer_recv, 3) == 0) {
			char word_str[100];
			char back_str[100];
			int num_operator_1, num_operator_2;
			int sum_result;
			char sum_result_str = "";

			strcpy(word_str, (buffer_recv + 4)); //remove o ADD

			//Gets both ints
			num_operator_1 = atoi(&word_str[0]); //Now that ADD's been removed, num1 is in this position			
			num_operator_2 = atoi(&word_str[2]);	//Same for num2

			//Now add them both and save in sum
			sum_result = num_operator_1 + num_operator_2;

			//Convert sum to string to send
			sum_result_str = sum_result - '0';

			//Notifies server about the ADD command received
			printf("Received ADD command\n");

			strcpy(back_str, sum_result_str);

			//Sends back the result
			send(client_socket, back_str, strlen(back_str), 0);
			send(client_socket, "\n", strlen("\n"), 0);	//And a '\n' char

		} else if (strncmp("RNDNUM", buffer_recv, 3) == 0) {
			int rand_num = rand() % 1000;
			sprintf(buffer_send, "%d\n", rand_num);
			printf("Received RNDNUM: %d\n", rand_num);
			send(client_socket, buffer_send, strlen(buffer_send),0);
		}

	}

	return 0;



}

int main(void) {

        int server_socket;
        struct sockaddr_in server_addr;
        pthread_t client_threads[MAX_CLIENTS];
        int client_counter = 0;

	inicializa_monitor(&_log);

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
		perror("bind() error!");
		exit(1);
	}

	int listen_res = listen(server_socket, MAX_CLIENTS);

	if (listen_res == -1) {
		perror("listen() error!");
		exit(1);
	}

        printf("Waiting for new connections on port %d...\n", SERVER_PORT);

        while(1) {
		int client_socket = accept(server_socket, NULL, NULL);

		pthread_create(&client_threads[client_counter], NULL,
				handle_client, (void*)client_socket);

		client_counter = ((client_counter + 1) % MAX_CLIENTS);
	}

	close(server_socket);

	finaliza_monitor(&_log);

	return 0;
}

//accept() é uma função bloqueante!!! (executar dentro de um loop (while(true)))
//cplusplus for docs
//watch out for lock -> unlokc cycle
//implementar um server com um monitor
//esse monitor vai recebr comandos, que a gente trata oq faz
//pedir diferentes comandos
//prova parecida com essa implementação