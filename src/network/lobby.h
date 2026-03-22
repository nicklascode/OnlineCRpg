#include "../entites/player.h"
#include "global_net.h"

#ifndef LOBBY_H
#define LOBBY_H

extern Client_Info lobby_clients_list[MAX_CLIENTS];
extern u8 num_clients_in_lobby;

void add_client_to_lobby(Client_Info* client_info);
void remove_client_from_lobby(u8 client_id);

Client_Info* get_client_info(u8 client_id);
Client_Info* get_all_clients(int* out_count);

#endif // !LOBBY_H