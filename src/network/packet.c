#include "packet.h"
#include <stdio.h>
#include <string.h>
#include "../util/logger.h"
#include "../util/compress_util.h"

int receive_packet(int socket, Packet *packet) {
    int received = recv(socket, (char*)&packet->type, sizeof(u8), 0); // Type
    if (received <= 0) {
        return -1;
    }

    received = recv(socket, (char*)&packet->length, sizeof(u16), 0); // Length
    if (received <= 0) {
        return -1;
    }

    int total_received = 0;
    while (total_received < packet->length) {
        int received = recv(socket, (char*)packet->payload + total_received, packet->length - total_received, 0); // Payload
        if (received <= 0) {
            return -1; // Connection closed or error
        }

        total_received += received;
        DEBUG_LOG("Received %d bytes of payload, total received: %d/%d", received, total_received, packet->length);
    }

    return 1; // Packet received successfully
}

void send_packet(int socket, Packet *packet) {
    int total_sent = 0;
    int packet_size = packet->length + sizeof(u8) + sizeof(u16);
    unsigned char send_buffer[sizeof(u8) + sizeof(u16) + MAX_PACKET_BUFFER]; // Buffer to hold the entire packet for sending
    DEBUG_LOG("Preparing to send packet of type %d with size %d", packet->type, packet->length);

    // Serialize packet
    send_buffer[0] = packet->type; // Type
    memcpy(send_buffer + 1, &packet->length, sizeof(u16)); // Length
    memcpy(send_buffer + 1 + sizeof(u16), packet->payload, packet->length); // Payload

    while (total_sent < packet_size) {
        int sent = send(socket, (const char*)send_buffer + total_sent, packet_size - total_sent, 0);
        if (sent == SOCKET_ERROR) {
            ERROR_LOG("Failed to send packet: %d", WSAGetLastError());
            return; // Connection closed or error
        }

        total_sent += sent;
    }

    DEBUG_LOG("Sent packet of type %d with size %d", packet->type, packet->length);
}

void serialize_packet(void* src, u8 type, Packet* dst, size_t size) {
    DEBUG_LOG("Serializing packet of type %d with size %d", type, size);
    dst->type = type;
    dst->length = (u16)size;

    memcpy(dst->payload, src, size);
}

void deserialize_packet(const Packet* src, void* dst, size_t size) {
    DEBUG_LOG("Deserializing packet of type %d with size %d", src->type, size);
    memcpy(dst, src->payload, size);
}