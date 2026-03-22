#include "lobby.h"

Client_Info lobby_clients_list[MAX_CLIENTS];
u8 num_clients_in_lobby = 0;

void add_client_to_lobby(Client_Info* client_info) {
    if (num_clients_in_lobby >= MAX_CLIENTS) {
        ERROR_LOG("Lobby is full. Cannot add more clients.");
        return;
    }
    lobby_clients_list[num_clients_in_lobby++] = *client_info;
    DEBUG_LOG("Added client to lobby: %s (ID: %d)", client_info->username, client_info->client_id);
}

void remove_client_from_lobby(u8 client_id) {
    for (u8 i = 0; i < num_clients_in_lobby; i++) {
        if (lobby_clients_list[i].client_id == client_id) {
            // Shift remaining clients down
            for (u8 j = i; j < num_clients_in_lobby - 1; j++) {
                lobby_clients_list[j] = lobby_clients_list[j + 1];
            }
            num_clients_in_lobby--;
            DEBUG_LOG("Removed client from lobby: ID %d", client_id);
            return;
        }
    }
    ERROR_LOG("Client ID %d not found in lobby.", client_id);
}

Client_Info* get_client_info(u8 client_id) {
    for (u8 i = 0; i < num_clients_in_lobby; i++) {
        if (lobby_clients_list[i].client_id == client_id) {
            return &lobby_clients_list[i];
        }
    }
    return NULL; // Not found
}