#include "../types.h"
#include <winsock2.h>
#include <ws2tcpip.h>

#ifndef PACKET_H
#define PACKET_H

#define MAX_PACKET_BUFFER 4096

typedef enum : u8
{
    Greeting = 1, // Join packet
    LobbyData // LobbyData, players, etc...
} Packets;


typedef struct packet {
    u8 id;
    u16 length; // Length of the payload
    unsigned char buffer[MAX_PACKET_BUFFER]; // Raw payload buffer
} Packet;


void send_packet(int socket, Packet *packet);
int receive_packet(int socket, Packet *packet);


// Generic serialization/deserialization
void serialize_packet(void* src, u8 type, Packet* dst, size_t size);
void deserialize_packet(const Packet* src, void* dst, size_t size);

/* PACKET TYPES [STRUCTS]*/

typedef struct playerData_packet {
    char username[128];
    u8 client_id; // Ignore when sending from client!
} PlayerDataPacket;

#endif