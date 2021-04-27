/***
 * Here are defined packet structs and everything that applies to them.
 *
 ***/

/*
  TODO: Get rid of magic numbers in game field state packet
  TODO: Moveable object packet implementation
  TODO: Player info packet implementation
  TODO: Ping packet implementation
  TODO: Packet decoder and encoder implementation
*/

/* Client->Server packets */
/* Player identification packet
   packet_id 0x00
*/
struct packet_player_id {
  unsigned char protocol_version;
  char player_name[32];
  char player_color;
};

/* Player input packet
   packet_id 0x01
*/
struct packet_player_input {
  char velocity_x;
  char velocity_y;
  unsigned char dynamite_put;
};

/* Chat message packet
   packet_id 0x02
*/
struct packet_client_chat_msg {
  char msg[256];
};

/* Ping packet
*/

/* Server->Client packets */

/* Server identification packet
   packet_id 0x80
   is_accepted contains the player id if the connection is accepted,
   otherwise 0.
*/
struct packet_server_id {
  unsigned char protocol_version;
  unsigned int is_accepted;
};

/* Game field state packet.
   packet_id 0x81
*/
struct packet_game_field_state {
  unsigned char width; /* 15 */
  unsigned char height; /* 13 */
  unsigned char block_id[15 * 13];
};

/* Moveable object update packet
   packet_id 0x82
*/
struct packet_moveable_obj_update {
};

/* Server chat message packet
   packet_id 0x83
*/
struct packet_server_chat_msg {
  unsigned char type;
  char msg[256];
};

/* Player information packet
   packet_id 0x84
*/

struct packet_player_info {

};
