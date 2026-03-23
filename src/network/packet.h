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

typedef struct packet_chunk {
    u8 type; // Packet type identifier
    u8 id; // Id
    u8 index; // Ie wich index of the packet we are
    u16 length; // Length of the payload
    unsigned char buffer[MAX_PACKET_BUFFER]; // Raw payload buffer
} Packet_chunk;

typedef struct packet {
    u8 type;
    u16 lenght;
    unsigned char* buffer;
} Packet;

void send_packet(int socket, Packet *packet);
int receive_packet(int socket, Packet *packet);

// Generic serialization/deserialization
void serialize_packet(void* src, u8 type, Packet* dst, size_t size);
void deserialize_packet(const Packet* src, void* dst, size_t size);

// Packet split
Packet_chunk slice_chunk(Packet* src, int start, size_t lenght); // Slice a part of the packet into a chunk


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