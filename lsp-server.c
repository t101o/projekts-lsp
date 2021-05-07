#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>

#include <unistd.h>

#include <pthread.h>

#include "packets.h"

// For now port is 1234
#define PORT 1234

int timer() {
	
	
}

void generate_map() {
	
}

void packetIn_sort() {
	
}

void packetOut_indentify() {
	
}


void packetOut_sendMap() {
	
}

void packetOut_updateField() {
	
}

void packetOut_chat() {
	
}

void packetOut_playerStats() {
	
}

int playerAlive() {
	
}

void player_connect() {
	int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
       
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
       
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
       
    if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    while (1) {
    valread = read(new_socket, buffer, 1024);
    printf("%s\n", buffer);
    send(new_socket , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    // After using buffer, it needs to be cleared
    memset(buffer, 0, 1024);
    }
    return 0;
}


int main() {
	/*struct packet_player_input ppi = {'a', 'd', 's'};
	char msgg = ppi.velocity_x;
	
	// creating server socket
	int server_socket;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	
	// def server adress
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9002);
	server_address.sin_addr.s_addr = INADDR_ANY;
	
	// bing socket to client
	bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));
	
	listen(server_socket, 5);
	
	
	int client_socket;
	client_socket = accept(server_socket, NULL, NULL);
	
	send(client_socket, &msgg, sizeof(msgg), 0);
	
	
	
	printf("Hello LSP\n");
	printf("ppi is: %c; %c; %c;\n", ppi.velocity_x, ppi.velocity_y, ppi.dynamite_put);*/
	
	player_connect();
	
}
