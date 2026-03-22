#include "../types.h"
#include "nettypes.h"
#include "global_net.h"
#include <winsock2.h>
#include <ws2tcpip.h>

#ifndef PACKET_H
#define PACKET_H

#define MAX_PACKET_BUFFER 4096

typedef enum : u8
{
    C_Greeting = 1, // Join packet
    S_LobbyData,
    C_RequestSpawn,
    S_SpawnEntity,
    S_UpdateEntity,
    C_MoveEntity,

} Packets;


typedef struct packet {
    u8 id; // Packet type identifier
    u8 isCompressed; // Flag to indicate if the payload is compressed (1 byte)
    u16 length; // Length of the payload
    unsigned char buffer[MAX_PACKET_BUFFER]; // Raw payload buffer
} Packet;


void send_packet(int socket, Packet *packet);
int receive_packet(int socket, Packet *packet);

// Generic serialization/deserialization
void serialize_packet(void* src, u8 type, Packet* dst, size_t size);
void deserialize_packet(const Packet* src, void* dst, size_t size);

void compress_packet(Packet* packet);
void decompress_packet(Packet* packet);

/* PACKET TYPES [STRUCTS]*/

typedef struct LobbyDataPacket {
    u8 num_players;
    Client_Info players[MAX_CLIENTS];
} LobbyDataPacket;

typedef struct SpawnEntityPacket {
    u32 entity_id;
    u8 entity_type;
    float x, y;

    u16 data_size; // Size of entity_data
    unsigned char entity_data[5012];
} SpawnEntityPacket;

typedef enum : u8 {
    REQUEST_SPAWN_PLAYER = 1,
} RequestSpawnType;

typedef struct RequestSpawnPacket {
    RequestSpawnType spawn_type;

} RequestSpawnPacket;

#endif