#include <raylib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 1234
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

struct player {
  Rectangle rectangle;
  Vector2 position;
};

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
  /*
  while (1) {
    char msg[256];
    char buff[1024];
    fgets(msg, 256, stdin);
    if (write(server_socket, msg, strlen(msg)) == -1) {
      perror("*ERROR*");
      exit(EXIT_FAILURE);
    }

    int read_status;
    read_status = read(server_socket, buff, 1024);
    if (read_status == -1) {
      perror("*ERROR*");
      exit(EXIT_FAILURE);
    } else if (read_status > 0) {
      fprintf(stdout, "%s\n", buff);
      memset(buff, 0, 1024);
    }
  }
  */
  
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "BOMBER DUDE");

  struct player player1;
  struct player player2;
  struct player player3;
  struct player player4;

  Texture2D textures = LoadTexture("texmap.png");

  float tile_height = textures.height/6;
  float tile_width = textures.width/15;

  /* Player data */
  player1.position.x = 0.0f;
  player1.position.y = 0.0f;
  player1.rectangle.x = 0.0f;
  player1.rectangle.y = tile_height;
  player1.rectangle.width = tile_width;
  player1.rectangle.height = tile_height;

  player2.position.x = SCREEN_WIDTH - tile_width;
  player2.position.y = 0.0f;
  player2.rectangle.x = 0.0f;
  player2.rectangle.y = tile_height * 2;
  player2.rectangle.width = tile_width;
  player2.rectangle.height = tile_height;
  /*
  player3.position = {0.0f, SCREEN_HEIGHT - tile_height};
  player3.rectangle = {0.0f, tile_height * 3, tile_width, tile_height};

  player4.position = {SCREEN_WIDTH - tile_width, SCREEN_HEIGHT - tile_height};
  player4.rectangle = {0.0f, tile_height * 4, tile_width, tile_height};
  */
  SetTargetFPS(60);
  
  while (!WindowShouldClose()) {
    /* Update variables */
    /*++frame_count;
    if (frame_count >= 60/8) {
      frame_count = 0;
      frameRec.x += texture_players.width/15;
      if (frameRec.x >= texture_players.width/15 * 10) {
	frameRec.x = 0;
	//frameRec.y += texture_players.height/6;
	//	if (frameRec.y >= texture_players.height/6 * 5) {
	//	  frameRec.y = texture_players.height/6;
	//	}
      }
      }*/

    /* Player movement */
    
    if (IsKeyDown(KEY_RIGHT)) {
      player1.position.x += 2.0f;
      player1.rectangle.x = tile_width * 4;
      if (player1.rectangle.width < 0) {
	player1.rectangle.width *= -1;
      }

    } else if (IsKeyDown(KEY_LEFT)) {
      player1.position.x -= 2.0f;
      player1.rectangle.x = tile_width * 4;
      if (player1.rectangle.width > 0) {
	player1.rectangle.width *= -1;
      }
    } else if (IsKeyDown(KEY_UP)) {
      player1.position.y -= 2.0f;
      player1.rectangle.x = 0;
    } else if (IsKeyDown(KEY_DOWN)) {
      player1.position.y += 2.0f;
      player1.rectangle.x = tile_width;
    }
    
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawTextureRec(textures, player1.rectangle, player1.position, WHITE);
    DrawTextureRec(textures, player2.rectangle, player2.position, WHITE);
    /*DrawTextureRec(textures, player3.rectangle, player3.position, WHITE);
      DrawTextureRec(textures, player4.rectangle, player4.position, WHITE);*/
    EndDrawing();
  }
  CloseWindow();
}
