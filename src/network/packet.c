#include <stdio.h>
#include "packet.h"
#include <winsock2.h>
#include <stdio.h>
#include <string.h>
#include "../util/logger.h"
#include "../util/compress_util.h"

int receive_packet(int socket, Packet_chunk *packet) {
    // Read the packet header (ID, isCompressed, and length)
    int received = recv(socket, (char*)&packet->type, 1, 0);
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

void send_packet(int socket, Packe *packet) {
    int total_sent = 0;
    // Now include isCompressed as a single byte after id
    int packet_size = sizeof(packet->type) + sizeof(packet->isCompressed) + sizeof(packet->length) + packet->length;
    unsigned char send_buffer[sizeof(Packet_chunk)];

    // Serialize the packet into a contiguous buffer
    send_buffer[0] = packet->type;
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
    dst->type = type;
    dst->lenght = (u16)size;

    int chunkIdStart = rand() % 256; // Random chunk ID
    if(size >= MAX_PACKET_BUFFER) {
        int chunkSize = 1;
        Packet_chunk* chunks = (Packet_chunk*)malloc(chunkSize * sizeof(Packet_chunk));
        int lastEndLenght = 0;

        for(int i = 0; i < size; i++) {
            if(lastEndLenght <= size) {
                int dstLeft = size - lastEndLenght;
                int copySize = dstLeft > MAX_PACKET_BUFFER ? MAX_PACKET_BUFFER : dstLeft; // How much we will copy, could be dstLeft, if there is less data in payload left then MAX_PACKET_BUFFER

                // Slice
                Packet_chunk chunk = slice_chunk((Packet*)src, lastEndLenght, copySize);
                chunk.id = (chunkIdStart + i);
                chunks[i] = chunk;
                lastEndLenght += copySize;

                // Add new chunk to array
                chunkSize++;
                chunks = (Packet_chunk*)realloc(chunks, chunkSize * sizeof(Packet_chunk));
                DEBUG_LOG("Created chunk %d with size %d (index %d)", chunk.id, chunk.length, chunk.index);
            } else {
                break;
            }
        }
    }

    memcpy(dst->buffer, src, size);
}

void deserialize_packet(const Packet* src, void* dst, size_t size) {
    if(src->isCompressed) {
        DEBUG_LOG("Packet is compressed, decompressing...");
        Packet_chunk temp_packet = *src; // Create a copy to decompress
        decompress_packet(&temp_packet);

        memcpy(dst, temp_packet.buffer, size);
        return;
    }

    memcpy(dst, src->buffer, size);
}

Packet_chunk slice_chunk(Packet* src, int start, size_t lenght) {
    const char* cpyPayload = (char*)malloc(src->lenght * sizeof(char));
    memcpy(cpyPayload, src->buffer, lenght);

    Packet_chunk chunk;
    chunk.type = src->type;
    chunk.id = 0; // We will set it after
    chunk.index = start / MAX_PACKET_BUFFER; // Calculate the index based on the start position
    chunk.length = (u16)lenght;
    
    int index = 0;
    while(index < lenght) {
        int copySize = (lenght - index) > MAX_PACKET_BUFFER ? MAX_PACKET_BUFFER : (lenght - index);
        memcpy(chunk.buffer + index, cpyPayload + index, copySize);
        index += copySize;
    }

    free(cpyPayload);
    return chunk;
}
