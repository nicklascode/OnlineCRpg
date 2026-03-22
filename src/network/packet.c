#include <stdio.h>
#include "packet.h"
#include <winsock2.h>
#include <string.h>
#include "../util/logger.h"
#include "../util/compress_util.h"

int receive_packet(int socket, Packet *packet) {
    // Read the packet header (ID, isCompressed, and length)
    int received = recv(socket, (char*)&packet->id, 1, 0);
    if (received <= 0) {
        return -1; // Connection closed or error
    }
    unsigned char isCompressedByte = 0;
    received = recv(socket, (char*)&isCompressedByte, 1, 0);
    if (received <= 0) {
        return -1; // Connection closed or error
    }
    packet->isCompressed = isCompressedByte ? 1 : 0;
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
    // Now include isCompressed as a single byte after id
    int packet_size = sizeof(packet->id) + sizeof(packet->isCompressed) + sizeof(packet->length) + packet->length;
    unsigned char send_buffer[sizeof(Packet)];

    // Serialize the packet into a contiguous buffer
    send_buffer[0] = packet->id;
    send_buffer[1] = packet->isCompressed ? 1 : 0;
    memcpy(send_buffer + 2, &packet->length, sizeof(packet->length));
    memcpy(send_buffer + 2 + sizeof(packet->length), packet->buffer, packet->length);

    // Send the entire packet (header + isCompressed + payload)
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

    if(size >= MAX_PACKET_BUFFER) {
        DEBUG_LOG("Size of packet [%d], exceeds capacity %d", size, MAX_PACKET_BUFFER);
        DEBUG_LOG("Gonna try and compress it");
        compress_packet(dst);
        return;
    }

    memcpy(dst->buffer, src, size);
}

void deserialize_packet(const Packet* src, void* dst, size_t size) {
    if(src->isCompressed) {
        DEBUG_LOG("Packet is compressed, decompressing...");
        Packet temp_packet = *src; // Create a copy to decompress
        decompress_packet(&temp_packet);
        if(temp_packet.length != size) {
            DEBUG_LOG("Decompressed packet size [%d] does not match expected size %d", temp_packet.length, size);
            return;
        }

        memcpy(dst, temp_packet.buffer, size);
        return;
    }

    memcpy(dst, src->buffer, size);
}

void compress_packet(Packet* packet) {
    int compressed_size = 0;
    unsigned char* compressed_data = compress_data(packet->buffer, packet->length, &compressed_size);
    if (compressed_data) {
        if (compressed_size < MAX_PACKET_BUFFER) {
            memcpy(packet->buffer, compressed_data, compressed_size);
            packet->length = (u16)compressed_size;
            packet->isCompressed = 1;
        } else {
            DEBUG_LOG("Compressed data size [%d] exceeds buffer capacity %d", compressed_size, MAX_PACKET_BUFFER);
        }
        free(compressed_data);
    } else {
        DEBUG_LOG("Failed to compress packet data");
    }
}

void decompress_packet(Packet* packet) {
    int decompressed_size = 0;
    unsigned char* decompressed_data = decompress_data(packet->buffer, packet->length, &decompressed_size);
    if (decompressed_data) {
        if (decompressed_size < MAX_PACKET_BUFFER) {
            memcpy(packet->buffer, decompressed_data, decompressed_size);
            packet->length = (u16)decompressed_size;
            packet->isCompressed = 0;
        } else {
            DEBUG_LOG("Decompressed data size [%d] exceeds buffer capacity %d", decompressed_size, MAX_PACKET_BUFFER);
        }
        free(decompressed_data);
    } else {
        DEBUG_LOG("Failed to decompress packet data");
    }
}
