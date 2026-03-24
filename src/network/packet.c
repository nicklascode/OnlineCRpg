#include <stdio.h>
#include "packet.h"
#include <winsock2.h>
#include <stdio.h>
#include <string.h>
#include "../util/logger.h"
#include "../util/compress_util.h"

int receive_packet(int socket, Packet *packet) {
    int received = recv(socket, (char*)&packet->type, sizeof(u8), 0); // Type
    if (received <= 0) {
        return -1;
    }

    received = recv(socket, (char*)&packet->id, sizeof(u32), 0); // ID
    if (received <= 0) {
        return -1;
    }

    received = recv(socket, (char*)&packet->length, sizeof(u16), 0); // Length
    if (received <= 0) {
        return -1;
    }

    int total_received = 0;
    while (total_received < packet->length) {
        int received = recv(socket, (char*)packet->chunks + total_received, packet->length - total_received, 0); // Payload
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
    int packet_size = packet->length + sizeof(u8) + sizeof(u32) + sizeof(u16);
    unsigned char send_buffer[sizeof(Packet) + packet->length]; // Buffer to hold the entire packet for sending
    DEBUG_LOG("Preparing to send packet of type %d with size %d", packet->type, packet->length);

    // Serialize packet
    send_buffer[0] = packet->type; // Type
    memcpy(send_buffer + 1, &packet->id, sizeof(u32)); // ID
    memcpy(send_buffer + 1 + sizeof(u32), &packet->length, sizeof(u16)); // Length
    memcpy(send_buffer + 1 + sizeof(u32) + sizeof(u16), packet->chunks, packet->length); // Payload

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
    u32 chunkId = rand() % 4294967295; // Random chunk ID

    dst->type = type;
    dst->id = chunkId;
    dst->length = (u16)size;

    if(size >= MAX_PACKET_BUFFER) {
        int chunkSize = 1;
        Packet_chunk* chunks = (Packet_chunk*)malloc(chunkSize * sizeof(Packet_chunk));
        int lastEndLenght = 0;

        for(int i = 0; i < size; i++) {
            if(lastEndLenght <= size) {
                int dstLeft = size - lastEndLenght;
                int copySize = dstLeft > MAX_PACKET_BUFFER ? MAX_PACKET_BUFFER : dstLeft; // How much we will copy, could be dstLeft, if there is less data in payload left then MAX_PACKET_BUFFER
                // Slice
                Packet_chunk chunk = slice_chunk(&src, lastEndLenght, copySize);
                chunks[i] = chunk;
                chunk.length = copySize;
                chunk.index = i;
                lastEndLenght += copySize;

                // Add new chunk to array
                chunkSize++;
                chunks = (Packet_chunk*)realloc(chunks, chunkSize * sizeof(Packet_chunk));
                DEBUG_LOG("Created chunk %d with size %d", i, copySize);
            } else {
                break;
            }
        }

        dst->chunks = chunks;
    }
    else {
        dst->chunks = (Packet_chunk*)malloc(sizeof(Packet_chunk));
        dst->chunks[0].index = 0;
        dst->chunks[0].length = (u16)size;
        memcpy(dst->chunks[0].buffer, src, size);
        DEBUG_LOG("Serialized packet into single chunk with size %d", size);
    }
}

void deserialize_packet(const Packet_chunk* src, Packet* dst, size_t size) {
    dst->type = src->index;
    dst->length = (u16)size;
    memcpy(dst->chunks, src->buffer, size);
}

Packet_chunk slice_chunk(Packet* src, int start, size_t lenght) {
    char* cpyPayload = (char*)malloc(src->length * sizeof(char));
    memcpy(cpyPayload, src->chunks, lenght);

    Packet_chunk chunk;
    
    int index = 0;
    while(index < lenght) {
        int copySize = (lenght - index) > MAX_PACKET_BUFFER ? MAX_PACKET_BUFFER : (lenght - index);
        memcpy(chunk.buffer + index, cpyPayload + index, copySize);
        index += copySize;
    }

    free(cpyPayload);
    return chunk;
}
