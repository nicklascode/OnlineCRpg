#include <stdio.h>
#include "packet.h"
#include <winsock2.h>
#include <string.h>

int receive_packet(int socket, Packet *packet) {
    // Read the packet header (ID and length)
    int received = recv(socket, (char*)&packet->id, 1, 0);
    if (received <= 0) {
        return -1; // Connection closed or error
    }
    received = recv(socket, (char*)&packet->length, sizeof(packet->length), 0);
    if (received <= 0) {
        return -1; // Connection closed or error
    }

    if (packet->length > MAX_PACKET_BUFFER) {
        DEBUG_LOG("Packet length exceeds buffer size: %d", packet->length);
        return -1; // Invalid packet length
    }

    // Read the packet payload
    int total_received = 0;
    while (total_received < packet->length) {
        received = recv(socket, (char*)packet->buffer + total_received, packet->length - total_received, 0);
        if (received <= 0) {
            return -1; // Connection closed or error
        }
        total_received += received;
    }

    return 1; // Packet received successfully
}

void send_packet(int socket, Packet *packet) {
    int total_sent = 0;
    int packet_size = sizeof(packet->id) + sizeof(packet->length) + packet->length;
    unsigned char send_buffer[sizeof(Packet)];

    // Serialize the packet into a contiguous buffer
    send_buffer[0] = packet->id;
    memcpy(send_buffer + 1, &packet->length, sizeof(packet->length));
    memcpy(send_buffer + 1 + sizeof(packet->length), packet->buffer, packet->length);

    // Send the entire packet (header + payload)
    while (total_sent < packet_size) {
        int sent = send(socket, (const char*)send_buffer + total_sent, packet_size - total_sent, 0);
        if (sent == SOCKET_ERROR) {
            DEBUG_LOG("Failed to send packet: %d", WSAGetLastError());
            return;
        }
        total_sent += sent;
    }
}

void serialize_packet(void* src, u8 type, Packet* dst, size_t size) {
    dst->id = type;
    dst->length = (u16)size;
    memcpy(dst->buffer, src, size);
}

void deserialize_packet(const Packet* src, void* dst, size_t size) {
    if (src->length == size) {
        memcpy(dst, src->buffer, size);
    }
}
