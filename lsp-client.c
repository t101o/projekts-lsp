#include <raylib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#include "packets.h"

#define PORT 1234
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

struct player {
  Rectangle rectangle;
  Vector2 position;
  char velocity;
};

struct bomb {
  Rectangle rectangle;
  Vector2 position;
};

struct bomb bombs[FIELD_HEIGHT * FIELD_WIDTH];

/* packets which are being sent */
union packet_player_id_union p_id;
union packet_player_input_union player1_input = {0xff00, 0x01, 0, 0, 0, 0};
union packet_client_chat_msg_union ccm;

/* packets which are being reveived */
union packet_server_id_union s_id;
union packet_game_field_state_union gfs;
union packet_moveable_obj_update_union mou;
union packet_server_chat_msg_union scm;
union packet_player_info_union pi;
union packet_ping_union p;

struct packet_game_field_state sgfs;
struct packet_moveable_obj_update smou;

struct player player1;
struct player player2;
struct player player3;
struct player player4;
char map[13 * 15];
int server_socket;

void *networking() {
  // For testing purposes
  
  while (1) {
    char msg[256];
    unsigned char buff[1024];
    //fgets(msg, 256, stdin);
    /*   if (write(server_socket, "Hello, server!\n", strlen("Hello, server!\n")) == -1) {
	perror("*ERROR*");
	exit(EXIT_FAILURE);
      }
    */
      int read_status;
      read_status = read(server_socket, buff, 1024);
      if (read_status == -1) {
	perror("*ERROR*");
	exit(EXIT_FAILURE);
      } else if (read_status > 0) {
	printf("%u\n", buff[1]);
	//if (buff[0] == 0xff && buff[1] == 0x00) {
	if (buff[2] == 0x80) {
	  memcpy(s_id.arr, buff, sizeof(s_id));
	  if (s_id.pack.is_accepted == 1) {
	    player1.position.x = 0.0f;
	    player1.position.y = 0.0f;
	  } else if (s_id.pack.is_accepted == 2) {
	    player1.position.x = SCREEN_WIDTH;
	    player1.position.y = 0.0f;
	  } else if (s_id.pack.is_accepted == 3) {
	    player1.position.x = SCREEN_WIDTH;
	    player1.position.y = SCREEN_HEIGHT;
	  } else if (s_id.pack.is_accepted == 4) {
	    player1.position.x = 0.0f;
	    player1.position.y = SCREEN_HEIGHT;
	  }
	} else if (buff[2] == 0x81) {
	  memcpy(gfs.arr, buff, sizeof(gfs));
	  sgfs = gfs.pack;
	  printf("Printing map\n");
	  for (int i = 0; i < FIELD_HEIGHT * FIELD_WIDTH; ++i) {
	    printf("%x ", sgfs.block_id[i]);
	    if (i % FIELD_WIDTH == 0) {
	      printf("\n");
	    }
	  }
	} else if (buff[2] == 0x82) {
	  memcpy(mou.arr, buff, sizeof(mou));
	  smou = mou.pack;
	  /* Moveable objects */
	} else if (buff[2] == 0x83) {
	  memcpy(scm.arr, buff, sizeof(scm));
	  struct packet_server_chat_msg sscm;
	  sscm = scm.pack;
	  printf("%s\n", sscm.msg);
	} else if (buff[2] == 0x84) {
	  memcpy(pi.arr, buff, sizeof(pi));
	  /* Player info */
	}else if (buff[2] == 0x85) {
	    
	    struct packet_ping ping;
	    memcpy(p.arr, buff, sizeof(p));
	    ping = p.pack;
	    //ping.type = ntohs(ping.type);
	    //decode_packet_ping(buff, &ping);
	    printf("PING %x %x %x!\n", ping.start, ping.type, p.arr[3]);
	    memset(buff, 0, 1024);
	    //unsigned char *temp = encode_packet_ping(buff, &ping);
	    write(server_socket, p.arr, sizeof(p));
	  }
	
	// }
      }
      memset(buff, 0, 1024);
      write(server_socket, player1_input.arr, sizeof(player1_input));
      sleep(1);
  }

}

void *game() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "BOMBER DUDE");
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
  player1.velocity = 1;
  
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


    /* Packet reveiving */
    /* Some packet sending */
    
    /* Player movement */
    
    if (IsKeyDown(KEY_RIGHT)) {
      player1_input.pack.velocity_x = player1.velocity;
      //player1.position.x += 2.0f;
      player1.rectangle.x = tile_width * 4;
      if (player1.rectangle.width < 0) {
	player1.rectangle.width *= -1;
      }

    } else if (IsKeyDown(KEY_LEFT)) {
      player1_input.pack.velocity_x = -player1.velocity;
      //player1.position.x -= 2.0f;
      player1.rectangle.x = tile_width * 4;
      if (player1.rectangle.width > 0) {
	player1.rectangle.width *= -1;
      }
    } else if (IsKeyDown(KEY_UP)) {
      player1_input.pack.velocity_y = -player1.velocity;
      //player1.position.y -= 2.0f;
      player1.rectangle.x = 0;
    } else if (IsKeyDown(KEY_DOWN)) {
      player1_input.pack.velocity_y = player1.velocity;
      //player1.position.y += 2.0f;
      player1.rectangle.x = tile_width;
    }
    
    if (IsKeyPressed(KEY_SPACE)) {
      player1_input.pack.dynamite_put = 1;
    }
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawTextureRec(textures, player1.rectangle, player1.position, WHITE);
    DrawTextureRec(textures, player2.rectangle, player2.position, WHITE);
    /*DrawTextureRec(textures, player3.rectangle, player3.position, WHITE);
      DrawTextureRec(textures, player4.rectangle, player4.position, WHITE);*/
    EndDrawing();
    /* Packet sending */
  }
  CloseWindow();

}

int main(int argc, char **argv) {

  if (argc != 2) {
    fprintf(stdout, "Too many arguments or missing server address!\n");
    exit(EXIT_FAILURE);
  }

  char *address = argv[1];
  
  server_socket = socket(AF_INET, SOCK_STREAM, 0);

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

  pthread_t game_thread;
  pthread_t networking_thread;

  if (pthread_create(&game_thread, NULL, &game, NULL) != 0) {
    perror("*ERROR*");
    exit(EXIT_FAILURE);
  }

  if (pthread_create(&networking_thread, NULL, &networking, NULL) != 0) {
    perror("*ERROR*");
    exit(EXIT_FAILURE);
  }
  pthread_join(game_thread, NULL);
  pthread_join(networking_thread, NULL);
}
