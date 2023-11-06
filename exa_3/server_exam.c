#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX_CLIENTS 5 //5 simulatenous connections
#define SERVER_PORT 8600
#define CLIENT_INFO_FILENAME "all_client_info.txt"
#define PASSWORD "senha1"

//Struct do monitor, guarda a info do cliente
typedef struct{
    pthread_mutex_t mutex;
    FILE* client_info_file;
    char client_ip_address[INET_ADDRSTRLEN];
} monitor_t;

//Overall monitor
monitor_t info_log_monitor;

//Init do monitor
void monitor_init(monitor_t* m) {
	pthread_mutex_init(&m->mutex, NULL);
}

//Finish do monitor, que também escreve a info do cliente
void monitor_finish(monitor_t* m) {
	pthread_mutex_destroy(&m->mutex);
	fclose(m->client_info_file);
}

//Writes client info Ip address on a file
void monitor_save_client_info(monitor_t* m, char* info_text) {
    pthread_mutex_lock(&m->mutex);
	m->client_info_file = fopen(CLIENT_INFO_FILENAME, "a");
	fprintf(m->client_info_file, "%s\n", info_text);
	fclose(m->client_info_file);
	pthread_mutex_unlock(&m->mutex);
}

//CLient handler
void* handle_client(void* p) {
	int client_socket = (int)p;
	char buffer_send_pass_prompt[1024];
	char buffer_received_passw[1024];
	char confirmation_correct_passw[1024] = "RIGHT PASSWORD!";
	char wrong_passw_msg[1024] = "WRONG PASSWORD, CLOSING CONNECTION.";

	//Send password prompt to client
	strcpy(buffer_send_pass_prompt, "INSERT PASSWORD:\n");
    send(client_socket, buffer_send_pass_prompt, strlen(buffer_send_pass_prompt), 0);

	//Receive client-inserted password
	recv(client_socket, buffer_received_passw, sizeof(buffer_received_passw), 0);

	//Comparing the client password to the server's password
	if(strcmp(buffer_send_pass_prompt, buffer_received_passw) != 0){
		printf("Rejected from client: %d (WRONG PASSWORD)\n", client_socket);

		//Sends rejection to client
		send(client_socket, wrong_passw_msg, strlen(wrong_passw_msg), 0);

		//And closes connection
		close(client_socket);
	}

	//If the password is right, it gets to here
	printf("New connection from client: %d (RIGHT PASSWORD)\n", client_socket);
	send(client_socket, confirmation_correct_passw, strlen(confirmation_correct_passw), 0);

	while(true) {
		char buffer_recv[1024];
		char buffer_send[1024];

	    recv(client_socket, buffer_recv, sizeof(buffer_recv), 0);
        buffer_recv[strlen(buffer_recv)] = '\0';

	    printf("Client: %s\n", buffer_recv);

		//END ends connection
		if(strncmp("END", buffer_recv, 3) == 0){
			strcpy(buffer_send, "CLOSING CONNECTION\n");
			send(client_socket, buffer_send, strlen(buffer_send), 0);

			close(client_socket);

            printf("Received END command from client: %d\n", client_socket);
			printf("Closing client: %d\n", client_socket);
			return 0;

		}else if(strncmp("ADD", buffer_recv, 3) == 0){
            //ADD <INT1> <INT2> case
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

			//Convert sum to string to send it back to client
			sum_result_str = sum_result - '0';

			//Notifies server about the ADD command received
			printf("Received ADD command from client: %d\n", client_socket);

			strcpy(back_str, sum_result_str);

			//Sends back the result
			send(client_socket, back_str, strlen(back_str), 0);
			send(client_socket, "\n", strlen("\n"), 0);	//And a '\n' char

		}else if(strncmp("SUB", buffer_recv, 3) == 0){
            //SUB <INT1> <INT2> case
			char word_str[100];
			char back_str[100];
			int num_operator_1, num_operator_2;
			int sum_result;
			char sub_result_str = "";

			strcpy(word_str, (buffer_recv + 4)); //remove o SUB

			//Gets both ints
			num_operator_1 = atoi(&word_str[0]); //Now that SUB's been removed, num1 is in this position			
			num_operator_2 = atoi(&word_str[2]);	//Same for num2

			//Now SUB them both and save in sum
			sum_result = num_operator_1 + num_operator_2;

			//Convert sum to string in order to send it back to client
			sub_result_str = sum_result - '0';

			//Notifies server about the SUB command received
			printf("Received SUB command from client: %d\n", client_socket);

			strcpy(back_str, sub_result_str);

			//Sends back the result
			send(client_socket, back_str, strlen(back_str), 0);
			send(client_socket, "\n", strlen("\n"), 0);	//And a '\n' char

		}else if(strncmp("MUL", buffer_recv, 3) == 0){
            //MUL <INT1> <INT2> case
			char word_str[100];
			char back_str[100];
			int num_operator_1, num_operator_2;
			int sum_result;
			char mul_result_str = "";

			strcpy(word_str, (buffer_recv + 4)); //remove o MUL

			//Gets both ints
			num_operator_1 = atoi(&word_str[0]); //Now that MUL's been removed, num1 is in this position			
			num_operator_2 = atoi(&word_str[2]);	//Same for num2

			//Now multiply them both and save in sum
			sum_result = num_operator_1 * num_operator_2;

			//Convert sum to string in order to send it back to client
			mul_result_str = sum_result - '0';

			//Notifies server about the MUL command received
			printf("Received MUL command from client: %d\n", client_socket);

			strcpy(back_str, mul_result_str);

			//Sends back the result
			send(client_socket, back_str, strlen(back_str), 0);
			send(client_socket, "\n", strlen("\n"), 0);	//And a '\n' char

		}else if(strncmp("DIV", buffer_recv, 3) == 0){
            //DIV <INT1> <INT2> case
			char word_str[100];
			char back_str[100];
			int num_operator_1, num_operator_2;
			int sum_result;
			char div_result_str = "";

			strcpy(word_str, (buffer_recv + 4)); //remove o DIV

			//Gets both ints
			num_operator_1 = atoi(&word_str[0]); //Now that DIV's been removed, num1 is in this position			
			num_operator_2 = atoi(&word_str[2]);	//Same for num2

			//Now divide them both and save in sum
			sum_result = num_operator_1 / num_operator_2;

			//Convert sum to string in order to send it back to client
			div_result_str = sum_result - '0';

			//Notifies server about the DIV command received
			printf("Received DIV command from client: %d\n", client_socket);

			strcpy(back_str, div_result_str);

			//Sends back the result
			send(client_socket, back_str, strlen(back_str), 0);
			send(client_socket, "\n", strlen("\n"), 0);	//And a '\n' char
		
        }else if(strncmp("LST", buffer_recv, 3) == 0){
            //LST case
			char word_str[1024];
			char back_str[1024];
            char command_header_str[1024] = "---COMANDOS---";
            char command_ADD_str[1024] = "ADD (ADD <INT1> <INT2>): sums two ints\n";
            char command_SUB_str[1024] = "SUB (SUB <INT1> <INT2>): subtracts among two ints\n";
            char command_MUL_str[1024] = "MUL (MUL <INT1> <INT2>): multiplies two ints\n";
            char command_DIV_str[1024] = "DIV (DIV <INT1> <INT2>): divides two ints\n";
            char command_LST_str[1024] = "LST: Shows this server's command list\n";
            char command_END_str[1024] = "END: Closes connection with server\n";

			//Notifies server about the LST command received
			printf("Received LST command from client: %d\n", client_socket);

            //Now sends the command list
            //Sends header to client
			strcpy(back_str, command_header_str);
            send(client_socket, back_str, strlen(back_str), 0);

            //Sends ADD instructions to client
			strcpy(back_str, command_ADD_str);
            send(client_socket, back_str, strlen(back_str), 0);

            //Sends SUB instructions to client
			strcpy(back_str, command_SUB_str);
            send(client_socket, back_str, strlen(back_str), 0);

            //Sends MUL instructions to client
			strcpy(back_str, command_MUL_str);
            send(client_socket, back_str, strlen(back_str), 0);

            //Sends DIV instructions to client
			strcpy(back_str, command_DIV_str);
            send(client_socket, back_str, strlen(back_str), 0);

            //Sends LST instructions to client
			strcpy(back_str, command_LST_str);
            send(client_socket, back_str, strlen(back_str), 0);

            //Sends END instructions to client
			strcpy(back_str, command_END_str);
            send(client_socket, back_str, strlen(back_str), 0);
		}

	}

	return 0;
}

int main(void)
{
    int server_socket;
    struct sockaddr_in server_addr;
    pthread_t client_threads[MAX_CLIENTS];
    int client_counter = 0;

    monitor_init(&info_log_monitor);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == -1){
        perror("socket() error!");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int bind_res = bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

	if(bind_res == -1){
		perror("bind() error!");
		exit(1);
	}

	int listen_res = listen(server_socket, MAX_CLIENTS);

	if(listen_res == -1){
		perror("listen() error!");
		return 1;
	}

    printf("Waiting for new connections on port %d...\n", SERVER_PORT);

    while(true){
		int client_socket = accept(server_socket, NULL, NULL);

		pthread_create(&client_threads[client_counter], NULL, handle_client, (void*)client_socket);

		client_counter = ((client_counter + 1) % MAX_CLIENTS);
	}

	close(server_socket);
	monitor_finish(&info_log_monitor);

	return 0;
}