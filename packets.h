/***
 * Here are defined packet structs and everything that applies to them.
 *
 ***/

#define FIELD_HEIGHT 13
#define FIELD_WIDTH 15

unsigned char checksum(unsigned char *packet, size_t size) {
  unsigned char checksum = 0;
  for (int i = 0; i < size-1; ++i) {
    checksum ^= packet[i];
  }

  return checksum;
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

/* Ping packet
   packet_id 0x85
*/
struct packet_ping {
  unsigned short start;
  unsigned char type;
  char checksum;
};

union packet_ping_union {
  unsigned char arr[sizeof(struct packet_ping)];
  struct packet_ping pack;
};
