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

#define PORT 8880
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 416
#define TILE_HEIGHT 32
#define TILE_WIDTH 32

struct player {
  Rectangle rectangle;
  Vector2 position;
  char direction;
  char velocity;
  unsigned char status;
};

struct bomb {
  Vector2 position;
  unsigned char status;
};

int input = 0;


/* packets which are being sent */
union packet_player_id_union p_id;
union packet_player_input_union player1_input;
union packet_client_chat_msg_union ccm;

/* packets which are being received */
union packet_server_id_union s_id;
union packet_game_field_state_union gfs;
union packet_moveable_obj_update_union mou;
union packet_server_chat_msg_union scm;
union packet_player_info_union pi;
union packet_ping_union p;

struct packet_game_field_state sgfs;
struct packet_moveable_obj_update smou;

struct player *player1;
struct player players[4];
struct bomb bombs[FIELD_HEIGHT * FIELD_WIDTH];
char map[FIELD_HEIGHT * FIELD_WIDTH];
int server_socket;

void DrawMap(Texture2D textures, Rectangle floor, Rectangle wall, Rectangle box, Rectangle fire) {
  Vector2 pos = {0, -TILE_HEIGHT};
  for (int i = 0; i < FIELD_WIDTH * FIELD_HEIGHT; ++i) {

    if (i % FIELD_WIDTH == 0) {
      pos.x = 0;
      pos.y += TILE_HEIGHT;
    }
    
    if (map[i] == 0x00) {
      DrawTextureRec(textures, floor, pos, WHITE);
    } else if (map[i] == 0x01) {
      DrawTextureRec(textures, wall, pos, WHITE);
    } else if (map[i] == 0x02) {
      DrawTextureRec(textures, box, pos, WHITE);
    } else if (map[i] == 0x03) {
      DrawTextureRec(textures, fire, pos, WHITE);
    }
    pos.x += TILE_WIDTH;
    //DrawTextureRec(textures, player1->rectangle, player1->position, WHITE);
  }
}

void *networking() {

  unsigned char buff[1024];
  p_id.pack.start = 0xff00;
  p_id.pack.type = 0x00;
  strcpy(p_id.pack.player_name, "Name");
  p_id.pack.player_color = 0;
  p_id.pack.checksum = 0; /* TODO */

  write(server_socket, p_id.arr, sizeof(p_id));
  
  // For testing purposes
  player1 = &players[0];
  while (1) {
    char msg[256];
    int read_status;
    read_status = read(server_socket, buff, 1024);
    if (read_status == -1) {
      perror("*ERROR*");
      exit(EXIT_FAILURE);
    } else if (read_status > 0) {
      //printf("%u\n", buff[1]);
      //if (buff[0] == 0xff && buff[1] == 0x00) {
      if (buff[2] == 0x80) {
	memcpy(s_id.arr, buff, sizeof(s_id));
	if (s_id.pack.is_accepted > 0) {
	  printf("Player id %d\n", s_id.pack.is_accepted);
	  player1 = &players[s_id.pack.is_accepted-1];
	}
	/* move to server */
	/*if (s_id.pack.is_accepted == 1) {
	  player1->position.x = TILE_WIDTH;
	  player1->position.y = TILE_HEIGHT;
	  } else if (s_id.pack.is_accepted == 2) {
	  player1->position.x = SCREEN_WIDTH - TILE_WIDTH;
	  player1->position.y = TILE_HEIGHT;
	  } else if (s_id.pack.is_accepted == 3) {
	  player1->position.x = SCREEN_WIDTH - TILE_WIDTH;
	  player1->position.y = SCREEN_HEIGHT - TILE_HEIGHT;
	  } else if (s_id.pack.is_accepted == 4) {
	  player1->position.x = TILE_WIDTH;
	  player1->position.y = SCREEN_HEIGHT - TILE_HEIGHT;
	  } */
      } else if (buff[2] == 0x81) {
	memcpy(gfs.arr, buff, sizeof(gfs));
	sgfs = gfs.pack;
	/*  printf("Printing map\n");
	    for (int i = 0; i < FIELD_HEIGHT * FIELD_WIDTH; ++i) {
	    printf("%x ", sgfs.block_id[i]);
	    if (i != 0 && i % (FIELD_WIDTH-1) == 0) {
	    printf("\n");
	    }
	    }*/
	/* Check checksum */
	memcpy(map, gfs.pack.block_id, FIELD_HEIGHT * FIELD_WIDTH);
      } else if (buff[2] == 0x82) {
	memcpy(mou.arr, buff, sizeof(mou));
	/* player */
	printf("IN\n");
	if (mou.pack.obj_type == 0x00) {
	  printf("PLAYERS %x\n", mou.pack.obj_id);
	  players[mou.pack.obj_id - 1].position.x = mou.pack.x;
	  players[mou.pack.obj_id - 1].position.y = mou.pack.y;
	  players[mou.pack.obj_id - 1].direction = mou.pack.direction;
	  players[mou.pack.obj_id - 1].status = mou.pack.status;

	  //printf("%d %d ");
	  /* bomb */
	} else if (mou.pack.obj_type == 0x01) {
	    
	}
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
    if (input) {
      printf("Sending...\n");
      write(server_socket, player1_input.arr, sizeof(player1_input));
      input = 0;
      printf("Sent!\n");
    }
    //      sleep(1);
  }

}

void *game() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "BOMBER DUDE");
  Texture2D textures = LoadTexture("texmap.png");

  float tile_height = textures.height/6;
  float tile_width = textures.width/15;
  
  Rectangle floor = {0, 0, tile_width, tile_height};
  Rectangle wall = {10 * tile_width, 5 * tile_height, tile_width, tile_height};
  Rectangle box = {9 * tile_width, 0, tile_width, tile_height};
  Rectangle bomb = {4 * tile_width, 5 * tile_height, tile_width, tile_height};
  Rectangle fire = {14 * tile_width, 5 * tile_height, tile_width, tile_height};
  
  /* temp Player data */
  players[0].position.x = 32;
  players[0].position.y = 32;
  players[0].rectangle.x = 0.0f;
  players[0].rectangle.y = tile_height;
  players[0].rectangle.width = tile_width;
  players[0].rectangle.height = tile_height;
  players[0].velocity = 1;
  players[0].status = 1;
  
  players[1].position.x = SCREEN_WIDTH - tile_width;
  players[1].position.y = 0.0f;
  players[1].rectangle.x = 0.0f;
  players[1].rectangle.y = tile_height * 2;
  players[1].rectangle.width = tile_width;
  players[1].rectangle.height = tile_height;
  players[1].status = 1;
  
  players[2].rectangle.x = 0.0f;
  players[2].rectangle.y = tile_height * 3;
  players[2].rectangle.width = tile_width;
  players[2].rectangle.height = tile_height;
  players[2].position.x = 40.0f;
  players[2].position.y = 40.0f;
  players[2].status = 1;
  
  players[3].rectangle.x = 0.0f;
  players[3].rectangle.y = tile_height * 4;
  players[3].rectangle.width = tile_width;
  players[3].rectangle.height = tile_height;
  players[3].position.x = 100.0f;
  players[3].position.y = 100.0f;
  players[3].status = 1;

  
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

    /*
    for (int i = 0; i < FIELD_WIDTH * FIELD_HEIGHT; ++i) {
      
      if (i % (FIELD_WIDTH) == 0) {
	printf("\n");
      }
      printf("%x ", map[i]);
    }
    printf("\n");
    */
    /* Packet reveiving */
    /* Some packet sending */
    
    /* Player movement */
    
    if (IsKeyDown(KEY_RIGHT)) {
      player1_input.pack.velocity_x = player1->velocity;
      player1_input.pack.velocity_y = 0;
      //player1->position.x += 2.0f;
      input = 1;
      player1->rectangle.x = tile_width * 4;
      if (player1->rectangle.width < 0) {
	player1->rectangle.width *= -1;
      }

    } else if (IsKeyDown(KEY_LEFT)) {
      player1_input.pack.velocity_x = -player1->velocity;
      //player1->position.x -= 2.0f;
      player1_input.pack.velocity_y = 0;
      input = 1;
      player1->rectangle.x = tile_width * 4;
      if (player1->rectangle.width > 0) {
	player1->rectangle.width *= -1;
      }
    } else if (IsKeyDown(KEY_UP)) {
      player1_input.pack.velocity_y = -player1->velocity;
      player1_input.pack.velocity_x = 0;
      //player1->position.y -= 2.0f;
      input = 1;
      player1->rectangle.x = 0;
    } else if (IsKeyDown(KEY_DOWN)) {
      player1_input.pack.velocity_y = player1->velocity;
      player1_input.pack.velocity_x = 0;
      input = 1;
      //player1->position.y += 2.0f;
      player1->rectangle.x = tile_width;
    }
    
    if (IsKeyPressed(KEY_SPACE)) {
      player1_input.pack.dynamite_put = 1;
      input = 1;
    }
    
    BeginDrawing();
    ClearBackground(RAYWHITE);
    /* Draw map */
    DrawMap(textures, floor, wall, box, fire);
    /*   Vector2 pos = {0, 0};
  for (int i = 0; i < FIELD_WIDTH * FIELD_HEIGHT; ++i) {

    if (i % (FIELD_WIDTH) == 0) {
      pos.x = 0;
      pos.y += floor.height;
    }
    
    if (map[i] == 0x00) {
      DrawTextureRec(textures, floor, pos, WHITE);
    } else if (map[i] == 0x01) {
      DrawTextureRec(textures, wall, pos, WHITE);
    } else if (map[i] == 0x02) {
      DrawTextureRec(textures, box, pos, WHITE);
    }
    pos.x += floor.width;
    //DrawTextureRec(textures, player1->rectangle, player1->position, WHITE);
    }*/

    /* Draw bombs */
    for (int i = 0; i < FIELD_HEIGHT * FIELD_WIDTH; ++i) {
      if (bombs[i].position.x >= 0 && bombs[i].position.y >= 0) {
	DrawTextureRec(textures, bomb, bombs[i].position, WHITE);
      }
    }

    /* Draw players */
    for (int i = 0; i < 4; ++i) {
      if (players[i].status > 0) {
	DrawTextureRec(textures, players[i].rectangle, players[i].position, WHITE);
      }
    }
    EndDrawing();
  }
  CloseWindow();
  return NULL;
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
  
  struct bomb empty;
  empty.position.x = -1.0f;
  empty.position.y = -1.0f;
  empty.status = 0;

  for (int i = 0; i < FIELD_WIDTH * FIELD_HEIGHT; ++i) {
    bombs[i] = empty;
  }

  player1_input.pack.start = 0xff00;
  player1_input.pack.type = 0x01;
  player1_input.pack.velocity_x = 0;
  player1_input.pack.velocity_y = 0;
  player1_input.pack.dynamite_put = 0;
  player1_input.pack.checksum = 0;
  
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
