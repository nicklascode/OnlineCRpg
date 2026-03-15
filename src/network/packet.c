#include "packet.h"
#include <winsock2.h>

Packet receive_packet(int socket, Packet* packet) {
    int bytes_received = recv(socket, packet->buffer, MAX_PACKET_BUFFER, 0);
    if (bytes_received > 0) {
        // First byte is the packet ID
        packet->id = (u8)packet->buffer[0];
    } else {
        packet->id = 0; // Indicate error or empty
    }
    
    return *packet;
}

void send_packet(int socket, Packet* packet) {
    // Assume packet->id is the first byte, followed by buffer data
    char send_buffer[MAX_PACKET_BUFFER];
    send_buffer[0] = (char)packet->id;
    memcpy(send_buffer + 1, packet->buffer, MAX_PACKET_BUFFER - 1);
    send(socket, send_buffer, MAX_PACKET_BUFFER, 0);
}
