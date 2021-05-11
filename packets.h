/***
 * Here are defined packet structs and everything that applies to them.
 *
 ***/

/*
  TODO: Ping packet implementation
*/

#define FIELD_HEIGHT 13
#define FIELD_WIDTH 15

unsigned char checksum(unsigned char *packet, size_t size) {
  unsigned char checksum = 0;
  for (int i = 0; i < size-1; ++i) {
    checksum ^= packet[i];
  }

  return checksum;
}

unsigned char *packet_encode(unsigned char *packet, size_t size) {
  unsigned char checksum = 0x00;
  unsigned char *packet_encoded = (unsigned char *) malloc(sizeof(unsigned char) * 2 * size);
  if (packet_encoded == NULL) {
    perror("*ERROR*");
    exit(EXIT_FAILURE);
  }

  memset(packet_encoded, 0, sizeof(char) * 2 * size);
  /* start of packet */
  packet_encoded[0] = packet[0];
  packet_encoded[1] = packet[1];
  
  checksum ^= packet_encoded[0];
  checksum ^= packet_encoded[1];
  
  int j = 2;
  for (int i = 2; i < size; ++i) {
    if (packet[i] == 0xff) {
      packet_encoded[j] = 0xff;
      packet_encoded[j+1] = 0xff;
      checksum ^= packet_encoded[j];
      checksum ^= packet_encoded[j];
      j += 2;
    } else {
      packet_encoded[j] = packet[i];
      checksum ^= packet_encoded[j];
      j += 1;
    }
  }
  packet_encoded[j] = checksum;

  return packet_encoded;
}


/* Size is obtainable from read syscall */
/* If error occurs then NULL is returned */
unsigned char *packet_decode(unsigned char *packet, size_t size) {
  /* Invalid packet */
  if (size < 3 || (packet[0] != 0xff && packet[1] != 0x00)) {
    return NULL;
  }

  unsigned char *packet_decoded = (unsigned char *) malloc(sizeof(unsigned char) * size); 
  /* Start decoding */
  int j = 0;
  for (int i = 0; i < size-1; ++i) {
    if (packet[i] == 0xff && packet[i+1] == 0xff) {
      packet_decoded[j] = 0xff;
      i++;
    } else if (packet[i] == 0xff && packet[i+1] != 0xff) {
      return NULL;
    } else {
      packet_decoded[j] = packet[i];
    }
    j++;
  }

  /* Checksums do not match */
  if (packet_decoded[j-1] != checksum(packet_decoded, j)) {
    return NULL;
  }

  return packet_decoded;
}



/* Client->Server packets */
/* Player identification packet
   packet_id 0x00
*/
struct packet_player_id {
  unsigned short start;
  unsigned char type;
  char player_name[32];
  char player_color;
  char checksum;
};

union packet_player_id_union {
  unsigned char arr[sizeof(struct packet_player_id)];
  struct packet_player_id pack;
};

/* Player input packet
   packet_id 0x01
*/
struct packet_player_input {
  unsigned short start;
  unsigned char type;
  char velocity_x;
  char velocity_y;
  unsigned char dynamite_put;
  char checksum;
};

union packet_player_input_union {
  unsigned char arr[sizeof(struct packet_player_input)];
  struct packet_player_input pack;
};

/* Chat message packet
   packet_id 0x02
*/
struct packet_client_chat_msg {
  unsigned short start;
  unsigned char type;
  char msg[256];
  char checksum;
};

union packet_client_chat_msg_union {
  unsigned char arr[sizeof(struct packet_client_chat_msg)];
  struct packet_client_chat_msg pack;
};

/* Server->Client packets */

/* Server identification packet
   packet_id 0x80
   is_accepted contains the player id if the connection is accepted,
   otherwise 0.
*/
struct packet_server_id {
  unsigned short start;
  unsigned char type;
  unsigned char protocol_version;
  unsigned int is_accepted;
  char checksum;
};

union packet_server_id_union {
  unsigned char arr[sizeof(struct packet_server_id)];
  struct packet_server_id pack;
};

/* Game field state packet.
   packet_id 0x81
*/
struct packet_game_field_state {
  unsigned short start;
  unsigned char type;
  unsigned char width; /* 15 */
  unsigned char height; /* 13 */
  unsigned char block_id[FIELD_HEIGHT * FIELD_WIDTH];
  char checksum;
};

union packet_game_field_state_union {
  unsigned char arr[sizeof(struct packet_game_field_state)];
  struct packet_game_field_state pack;
};

/* Moveable object update packet
   packet_id 0x82
*/
struct packet_moveable_obj_update {
  unsigned short start;
  unsigned char type;
  unsigned char obj_type;
  unsigned int obj_id;
  float x;
  float y;
  char direction;
  unsigned char status;
  char checksum;
};

union packet_moveable_obj_update_union {
  unsigned char arr[sizeof(struct packet_moveable_obj_update)];
  struct packet_moveable_obj_update pack;
};

/* Server chat message packet
   packet_id 0x83
*/
struct packet_server_chat_msg {
  unsigned short start;
  unsigned char type;
  unsigned char msg_type;
  char msg[256];
  char checksum;
};

union packet_server_chat_msg_union {
  unsigned char arr[sizeof(struct packet_server_chat_msg)];
  struct packet_server_chat_msg pack;
};

/* Player information packet
   packet_id 0x84
*/

struct packet_player_info {
  unsigned short start;
  unsigned char type;
  unsigned int player_id;
  char player_name[32];
  char player_color;
  unsigned int player_points;
  unsigned char player_lives;
  char checksum;
};

union packet_player_info_union {
  unsigned char arr[sizeof(struct packet_player_info)];
  struct packet_player_info pack;
};
  
struct packet_ping {
  unsigned short start;
  unsigned char type;
  char checksum;
};

union packet_ping_union {
  unsigned char arr[sizeof(struct packet_ping)];
  struct packet_ping pack;
};
/*
unsigned char *encode_packet_ping(unsigned char *buff,
				  struct packet_ping *pack) {
  buff[0] = (unsigned char) (pack->start);
  printf("encoding %u\n", buff[0]);
  buff[1] = (unsigned char) (pack->start);
  buff[2] = pack->type;
  buff[3] = (unsigned char) pack->checksum;

  return buff + 4;
}

unsigned char *decode_packet_ping(unsigned char *buff,
				  struct packet_ping *pack) {
  pack->start = (unsigned short) (buff[0] << 2);
  pack->start = (unsigned short) (pack->start | buff[1]);
  pack->type = buff[2];
  pack->checksum = (char) buff[3];

  return NULL;
}
*/
