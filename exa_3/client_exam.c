#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>

#define SERVER_PORT 8600


//Args are: password, command, int1, int2
int main(int argc, char* argv[])
{
	int client_socket;
	struct sockaddr_in server_address;
    char server_addr_str[1024];
    char file_name_str[1024];
	char buffer_recv[1024];
	char buffer_send[1024];
    char password_confirmation_from_server[1025];

    //Input filters
    if(argc < 4){
        printf("Not enough args!\n");
        printf("Args are: <password> <command> <int1> <int2> !\n");
        exit(1);
    }else if(argc > 4){
        printf("Too many args!\n");
        printf("Args are: <password> <command> <int1> <int2> !\n");
        exit(1);
    }

	client_socket = socket(AF_INET, SOCK_STREAM, 0);

    //Socket failsafe
	if(client_socket == -1){
		perror("socket() error!");
		exit(1);
	}

    //Populating server info
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(SERVER_PORT);
	server_address.sin_addr.s_addr = inet_addr(server_addr_str);

    //Connecting to server
	int res_connect = connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    //Server failsafe
	if(res_connect == -1){
		perror("connect() error!");
		exit(1);
	}

    //Notifies client that connection managed to be done
	printf("Connected to %s\n", server_addr_str);

    //Receives first message from server
    recv(client_socket, buffer_recv, sizeof(buffer_recv), 0);

    //Trying to send inserted password to it (it's on argv[0])
    strcpy(buffer_send, argv[0]);   //Copies it to sending-buffer
    printf("Sending inserted password: %s\n", buffer_recv);

    send(client_socket, buffer_send, sizeof(buffer_send), 0);   //Sending password to server

    //Counter just to recognize when the msg sent is the password (and to compare it)
    int msg_sent_counter = 0;

	while(true){
        //Comparing msg for first loop (first msg sent is password)
        if(msg_sent_counter == 0){
            //should test password first

            //Check the msg received from server after sending passord
            if(password_confirmation_from_server == )
        }

		recv(client_socket, buffer_recv, sizeof(buffer_recv), 0);
		printf("RECEIVED FROM SERVER: %s\n", buffer_recv);

		printf("SENT TO SERVER: %s\n", buffer_send);


	}

    close(client_socket);

    return 0;
}