//#include <raylib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 1234

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stdout, "Too many arguments or missing server address!\n");
    exit(EXIT_FAILURE);
  }

  char *address = argv[1];
  
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (server_socket == -1) {
    perror("*ERROR*");
    exit(EXIT_FAILURE);
  }
  
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  inet_pton(AF_INET, address, &server_address.sin_addr);
  
  if (connect(server_socket,
	      (struct sockaddr *) &server_address,
	      sizeof(server_address)) == -1) {
    perror("*ERROR*");
    exit(EXIT_FAILURE);
  }
  fprintf(stdout, "Connection successful with: %s:%d\n", address, PORT);

  // For testing purposes
  while (1) {
    char msg[256];
    char buff[1024];
    fgets(msg, 256, stdin);
    if (write(server_socket, msg, strlen(msg)) == -1) {
      perror("*ERROR*");
      exit(EXIT_FAILURE);
    }
    
    if (read(server_socket, buff, 1024) == -1) {
      perror("*ERROR*");
      exit(EXIT_FAILURE);
    }
    fprintf(stdout, "%s\n", buff);
    memset(buff, 0, 1024);
  }
  
}
