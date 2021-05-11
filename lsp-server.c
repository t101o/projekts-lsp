#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <time.h>

#include <netinet/in.h>

#include <unistd.h>

#include <pthread.h>

#include "packets.h"

#define PORT 1234

char map[15][13];


int timer(int type) {


}

void generate_map() {
	srand(time(NULL));
	int r = 0;
	char Q;
	int yy = 2;
	for(int y = 0; y < 13; y++) {
		for(int x = 0; x < 15; x++) {
			if (y == 0 || x == 0 || (x%2 == 0 && y%2 == 0) || x == 14 || y == 12) map[x][y] = '#';
			else map[x][y] = '-';
			r = rand() % 6;

			if (r == 1) Q = '1';
			else if (r == 2) Q = '2';
			else if (r == 3) Q = '3';
			else Q = '0';


			if ((y == 1 || y == 11) && ((x > 2 && x < 12) && x%2 == 0)) map[x][y] = Q;
			if (y%2 == 0 && y != 12 && y != 0 && (x != 1 && x != 13 && x%2 != 0)) map[x][y] = Q;
			if ((y == 3 || y == 5 || y == 7 || y == 9) && (x > 1 && x < 13 && x%2 == 0)) map[x][y] = Q;
			if ((x == 1 || x == 13) && (y == 4 || y == 6 || y == 8)) map[x][y] = Q;
		}
	}
}



void unpack() {

}

void packetIn_sort(int new_socket) {

}

void packetOut_indentify() {

}


void packetOut_sendMap(int new_socket) {
	unsigned char buff[1024];
	union packet_game_field_state_union pgfsu;
	struct packet_game_field_state pgfs;
	char bid = '\0';
	//char xx = '\0';
	int xx = 0;
	//char yy = '\0';
	pgfs.start = 0xff00;
	pgfs.type = 0x81;
	pgfs.width = 15;
	pgfs.height = 13;
	pgfs.checksum = pgfs.width ^ pgfs.height;
	for(int y = 0; y < 13; y++) {
		for(int x = 0; x < 15; x++) {

			//xx = x;
			//yy = y;

			//bid = map[x][y];
			//pgfs = {yy, xx, bid};

			//memcpy(pgfs.block_id, bid, 2);
			if (map[x][y] == 0x00) pgfs.block_id[xx] = 0x00;
			else if (map[x][y] == 0x01) pgfs.block_id[xx] = 0x01;
			else pgfs.block_id[xx] = 0x02;

			pgfs.checksum ^= pgfs.block_id[xx];
			xx++;
		}
	}
	pgfsu.pack = pgfs;
	memcpy(buff, pgfsu.arr, sizeof(pgfsu));
	send(new_socket, buff, sizeof(pgfsu), 0);
	printf("Map fragment sent!\n");
	memset(buff, 0, 1024);
	return;
	//send(new_socket , hello , strlen(hello) , 0 );
}

void packetOut_updateField() {

}

void packetOut_chat(int new_socket) {
	int valread;
	char buffer[1024] = {0};
  //char *chatmsg = "Hello from server";
	valread = read( new_socket , buffer, 1024);

	//char *chatmsg = read( new_socket , buffer, 1024);

  printf("From cilent: %s\n",buffer );
  send(new_socket , buffer , strlen(buffer) , 0 );
  printf("Chat message sent\n");
}

void packetOut_playerStats() {

}

int playerAlive() {

}

void updateClientInfoLocal() {

}

int abilityPlayerUpdate(int playerId, char Q) {

}

int player_connect() {
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
      //valread = read(new_socket, buffer, 1024);
      //printf("%s\n", buffer);
    union packet_ping_union pingu;
    struct packet_ping ping;
    ping.start = 0xff00;
    ping.type = 0x85;
    ping.checksum = ping.start ^ ping.type;
    pingu.pack = ping;
    unsigned char buff[1024];
    memcpy(buff, pingu.arr, sizeof(pingu));
    //unsigned char *temp = encode_packet_ping(buff, &ping);
    send(new_socket , buff , sizeof(pingu) , 0 );
    printf("Hello message sent %x\n", pingu.pack.type);
    // After using buffer, it needs to be cleared
    memset(buff, 0, 1024);
    sleep(1);
    }
    return 0;
	//valread = read(new_socket, buffer, 1024);
	return 0;
	/*
	while (1) {
	valread = read(new_socket, buffer, 1024);
	printf("%s\n", buffer);
	send(new_socket , hello , strlen(hello) , 0 );
	printf("Hello message sent\n");
	// After using buffer, it needs to be cleared
	memset(buffer, 0, 1024);
}*/
	//return 0;


	/*
	int main_socket;
	struct sockaddr_in server_address;
	int client_socket;
	struct sockaddr_in client_address;
	int client_address_size = sizeof(client_address);

	if ((main_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }
	printf("Main socket created\n");

	client_address.sin_family = AF_INET;
  client_address.sin_addr.s_addr = INADDR_ANY;
  client_address.sin_port = htons(PORT);
	*/

	/*
	int server_fd, new_socket;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);


  // Creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

    // Forcefully attaching socket to the port 8080
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons( PORT );

  // Forcefully attaching socket to the port 8080
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  if (listen(server_fd, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }
  if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
    perror("accept");
    exit(EXIT_FAILURE);
  }

	packetOut_chat(new_socket);*/
}


int main() {
	//char map[15][13];
	//Def

	generate_map(map);

	for(int y = 0; y < 13; y++) {
		for(int x = 0; x < 15; x++) {
			printf("%c ", map[x][y]);
		}
		printf("\n");
	}

	player_connect();

  //packetOut_chat(new_socket);

	//return 0;



	//player_connect();

}
