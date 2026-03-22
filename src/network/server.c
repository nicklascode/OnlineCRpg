#include "server.h"
#include "packet.h"
#include "network.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string.h>
#include "global_net.h"
#include "lobby.h"
#include "../entites/entity.h"


typedef struct {
    SOCKET socket;
    SOCKADDR_IN address;
    u8 clientId;
    int connected;
} ClientSocket;

Server server;
ClientSocket server_clients_list[MAX_CLIENTS];

void server_init() {
    // Init Winsock
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Create socket
    server.socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    server.address.sin_family = AF_INET;
    server.address.sin_addr.s_addr = INADDR_ANY;
    server.address.sin_port = htons(PORT);

    u_long mode = 1;
    ioctlsocket(server.socket, FIONBIO, &mode);

    // Bind socket
    bind(server.socket, (SOCKADDR*)&server.address, sizeof(server.address));
    listen(server.socket, SOMAXCONN);

    // Initialize client array
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        server_clients_list[i].socket = INVALID_SOCKET;
        server_clients_list[i].connected = 0;
        server_clients_list[i].clientId = i;
    }

    DEBUG_LOG("Server initialized and listening on port %d", PORT);
}

void server_handle_packets() {
    // Accept new clients
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (!server_clients_list[i].connected) {
            int addrSize = sizeof(server_clients_list[i].address);
            SOCKET newSocket = accept(server.socket, (SOCKADDR*)&server_clients_list[i].address, &addrSize);
            if (newSocket != INVALID_SOCKET) {
                server_clients_list[i].socket = newSocket;
                u_long mode = 1;
                ioctlsocket(server_clients_list[i].socket, FIONBIO, &mode);
                server_clients_list[i].connected = 1;
                DEBUG_LOG("Accepted new client connection (clientId=%d)", server_clients_list[i].clientId);
            }
        }
    }
    
// Handle packets for each connected client
 #pragma region Packets 
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (server_clients_list[i].connected && server_clients_list[i].socket != INVALID_SOCKET) {
            Packet packet;
            int received = 1;
            while (1) {
                received = receive_packet(server_clients_list[i].socket, &packet);
                if (received == 1) {
                    switch (packet.id) {
                        case C_Greeting: 
                        {
                            Client_Info greeting;
                            memcpy(&greeting, packet.buffer, sizeof(Client_Info));
                            DEBUG_LOG("Deserialized greeting from %s:", greeting.username);

                            // Add client to lobby
                            Client_Info new_client_info;
                            new_client_info.client_id = server_clients_list[i].clientId;
                            strcpy(new_client_info.username, greeting.username);

                            add_client_to_lobby(&new_client_info);

                            // Create lobbydata packet
                            LobbyDataPacket lobby_data;
                            lobby_data.num_players = num_clients_in_lobby;
                            memcpy(lobby_data.players, lobby_clients_list, sizeof(Client_Info) * num_clients_in_lobby);

                            // Send lobby data to client
                            Packet lobby_packet;
                            serialize_packet(&lobby_data, S_LobbyData, &lobby_packet, sizeof(LobbyDataPacket));
                            send_packet(server_clients_list[i].socket, &lobby_packet);
                            break;
                        }

                        case C_RequestSpawn:
                        {
                            RequestSpawnPacket spawn_request;
                            memcpy(&spawn_request, packet.buffer, sizeof(RequestSpawnPacket));
                            DEBUG_LOG("Received spawn request of type %d from clientId=%d", spawn_request.spawn_type, server_clients_list[i].clientId);

                            if(spawn_request.spawn_type == REQUEST_SPAWN_PLAYER) {
                                Entity* player_entity = create_entity(current_entity_manager, ENTITY_PLAYER, -1);
                                if(player_entity) {
                                    player_entity->position = (Vec2){100, 100};
                                    int player_texture_ids[] = {0};
                                    player_entity->sprite = create_sprite(player_texture_ids, 1);

                                    PlayerData player_data;
                                    player_data.client_id = server_clients_list[i].clientId;
                                    strcpy(player_data.username, lobby_clients_list[server_clients_list[i].clientId].username);
                                    player_data.livingData.health = 100;
                                    player_data.livingData.damage = 10;
                                    player_data.livingData.speed = 1;

                                    player_entity->data = NULL; // Not sent to client

                                    // Broadcast spawn to all clients
                                    SpawnEntityPacket spawn_packet_data;
                                    spawn_packet_data.entity_id = player_entity->ID;
                                    spawn_packet_data.entity_type = ENTITY_PLAYER;
                                    spawn_packet_data.x = player_entity->position.x;
                                    spawn_packet_data.y = player_entity->position.y;
                                    spawn_packet_data.data_size = sizeof(PlayerData);
                                    memcpy(spawn_packet_data.entity_data, &player_data, sizeof(PlayerData));

                                    DEBUG_LOG("Broadcasting spawn of player entity_id=%d for clientId=%d", spawn_packet_data.entity_id, server_clients_list[i].clientId);

                                    Packet spawn_packet;
                                    serialize_packet(&spawn_packet_data, S_SpawnEntity, &spawn_packet, sizeof(SpawnEntityPacket));

                                    for (int j = 0; j < MAX_CLIENTS; ++j) {
                                        if (server_clients_list[j].connected && server_clients_list[j].socket != INVALID_SOCKET) {
                                            send_packet(server_clients_list[j].socket, &spawn_packet);
                                        }
                                    }
                                } else {
                                    ERROR_LOG("Failed to create player entity for clientId=%d", server_clients_list[i].clientId);
                                }
                            }
                            break;
                        }

                        // ERRRRR
                        case 0:
                        default:
                            ERROR_LOG("Received unknown packet ID: %d from clientId=%d", packet.id, server_clients_list[i].clientId);
                            break;
                    }
                } else if (received == -1) {
                    // No data available, break loop
                    break;
                } else if (received == 0) {
                    // Disconnected
                    DEBUG_LOG("Client disconnected (clientId=%d)", server_clients_list[i].clientId);
                    closesocket(server_clients_list[i].socket);
                    server_clients_list[i].socket = INVALID_SOCKET;
                    server_clients_list[i].connected = 0;
                    break;
                }
            }
        }
    }

    #pragma endregion Packets
}

void server_shutdown() {
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (server_clients_list[i].socket != INVALID_SOCKET) {
            closesocket(server_clients_list[i].socket);
        }
    }
    closesocket(server.socket);
    WSACleanup();
}