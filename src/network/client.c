
#define NOGDI
#define NOMINMAX
#include <winsock2.h>
#include <ws2tcpip.h>

#include "client.h"
#include "packet.h"
#include "network.h"
#include "../util/logger.h"
#include "../util/math.h"
#include "../network/nettypes.h"
#include "../network/global_net.h"
#include "../network/lobby.h"
#include "../entites/entity.h"
#include "../entites/player.h"
#include "../network/global_net.h"

Client local_client;


void client_init() {
	// Initialize Winsock
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// Create a socket and connect to the server
	local_client.socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	local_client.address.sin_family = AF_INET;
	local_client.address.sin_addr.s_addr = inet_addr("127.0.0.1"); // Localhost for now
	local_client.address.sin_port = htons(PORT);

	   if (connect(local_client.socket, (SOCKADDR*)&local_client.address, sizeof(local_client.address)) == SOCKET_ERROR) {
		   ERROR_LOG("Failed to connect to server");
	   } else {
		   DEBUG_LOG("Connected to server");
		   // Set client socket to non-blocking
		   u_long mode = 1;
		   ioctlsocket(local_client.socket, FIONBIO, &mode);
	   }
}

void client_handle_packets() {
	static int greeting_sent = 0;
	if (local_client.socket == INVALID_SOCKET) {
		ERROR_LOG("Client socket is invalid");
		return;
	}
	if (!greeting_sent) {
		DEBUG_LOG("Sending greeting");
		Client_Info client_info_packet;
		memset(&client_info_packet, 0, sizeof(Client_Info));
		strcpy(client_info_packet.username, "ClientUser");
		client_info_packet.client_id = -1;

		Packet packet;
		serialize_packet(&client_info_packet, C_Greeting, &packet, sizeof(Client_Info));
		client_send_packet(&packet);

		DEBUG_LOG("Sent greeting packet");
		greeting_sent = 1;
	}

	// Receive packets from server
	Packet recv_packet;
	while (1) {
		int received = receive_packet(local_client.socket, &recv_packet);
		if (received == 1) {
			DEBUG_LOG("Received packet from server, id: %d", recv_packet.type);
			switch (recv_packet.type) {
				case S_LobbyData: {
					LobbyDataPacket lobby_data;
					deserialize_packet(&recv_packet, &lobby_data, sizeof(LobbyDataPacket));

					Client_Info* clients = lobby_data.players;
					u8 num_clients = lobby_data.num_players;

					memcpy(lobby_clients_list, clients, sizeof(Client_Info) * num_clients);
					memcpy(&num_clients_in_lobby, &num_clients, sizeof(u8));
					DEBUG_LOG("Updated lobby data: %d clients in lobby", num_clients);

					// Let's request player spawn after receiving lobby data
					RequestSpawnPacket spawn_request = {
						.spawn_type = REQUEST_SPAWN_PLAYER
					};

					Packet spawn_packet;
					serialize_packet(&spawn_request, C_RequestSpawn, &spawn_packet, sizeof(RequestSpawnPacket));
					client_send_packet(&spawn_packet);
					break;
				}

				case S_SpawnEntity: { // Handle spawn entity packet
					SpawnEntityPacket spawn_data;
					deserialize_packet(&recv_packet, &spawn_data, sizeof(SpawnEntityPacket));
					DEBUG_LOG("Received spawn entity packet: entity_id=%d, type=%d, x=%.2f, y=%.2f", spawn_data.entity_id, spawn_data.entity_type, spawn_data.x, spawn_data.y);

					if(spawn_data.entity_type == ENTITY_PLAYER) { // Handle player entity spawn
						Entity* player_entity = create_entity(current_entity_manager, ENTITY_PLAYER, spawn_data.entity_id);
						DEBUG_LOG("Created player entity with ID %d", player_entity ? player_entity->ID : -1);
						if(player_entity) {
							player_entity->position = (Vec2){spawn_data.x, spawn_data.y};
							int player_texture_ids[] = {0}; // Assuming texture ID 0 is valid
							player_entity->sprite = create_sprite(player_texture_ids, 1);

							if (spawn_data.data_size == sizeof(PlayerData)) {
								PlayerData* player_data = (PlayerData*)spawn_data.entity_data;
								DEBUG_LOG("Deserialized player data: client_id=%d, username=%s", player_data->client_id, player_data->username);
								
								PlayerData* player_data_copy = (PlayerData*)malloc(sizeof(PlayerData));
								memcpy(player_data_copy, player_data, sizeof(PlayerData));
								player_entity->data = player_data_copy;

								if(player_data_copy->client_id == global_network.local_client_id) {
									init_local_player(player_entity, player_data_copy);
									DEBUG_LOG("Initialized local player entity with ID %d", player_entity->ID);
								} else {
									init_player(player_entity, player_data_copy);
									DEBUG_LOG("Initialized remote player entity with ID %d", player_entity->ID);
								}
							} else {
								ERROR_LOG("SpawnEntityPacket player data size mismatch: got %d, expected %llu", spawn_data.data_size, (unsigned long long)sizeof(PlayerData));
							}
						} else {
							ERROR_LOG("Failed to create player entity for spawned entity_id=%d", spawn_data.entity_id);
						}
					}

					break;
				}

				case S_UpdateEntity: {

					break;
				}

				case C_MoveEntity: {
					MoveEntityPacket move_data;
					deserialize_packet(&recv_packet, &move_data, sizeof(MoveEntityPacket));
					DEBUG_LOG("Received move entity command for entity_id=%d to (%.2f, %.2f)", move_data.entity_id, move_data.new_x, move_data.new_y);

					Entity* entity = get_entity_by_id(current_entity_manager, move_data.entity_id);
					if(entity && entity->type == ENTITY_PLAYER) {
						entity->position.x = lerp(entity->position.x, move_data.new_x, 0.5f);
						entity->position.y = lerp(entity->position.y, move_data.new_y, 0.5f);
					} else {
						ERROR_LOG("Entity with ID %d not found or is not a player", move_data.entity_id);
					}
				}

				case 0:
				default:
					ERROR_LOG("Received unknown packet ID: %d", recv_packet.type);
					break;
			}
		} else if (received == -1) {
			// No data available, break loop
			break;
		} else if (received == 0) {
            ERROR_LOG("Server disconnected");
            client_shutdown();
            break;
		}
	}
}

void client_shutdown() {
	DEBUG_LOG("Disconnected from server");
	if (local_client.socket != INVALID_SOCKET) {
		closesocket(local_client.socket);
		local_client.socket = INVALID_SOCKET;
	}
	// Mark as disconnected so network_update() stops calling client_handle_packets
	extern Network global_network;
	global_network.isConnected = 0;
	WSACleanup();
}

void client_send_packet(Packet* packet) {
	if (local_client.socket != INVALID_SOCKET) {
		send_packet(local_client.socket, packet);
	} else {
		ERROR_LOG("Cannot send packet: client socket is invalid");
	}
}
