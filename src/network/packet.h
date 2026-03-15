#include "../types.h"

#ifndef PACKET_H
#define PACKET_H

#define MAX_PACKET_BUFFER 4096

typedef enum : u8
{
    Greeting = 1
} Packets;

typedef struct packet 
{
    u8 id;
    char buffer[MAX_PACKET_BUFFER];
} Packet;

#endif

void send_packet(int socket, Packet* packet);
Packet receive_packet(int socket, Packet* packet);