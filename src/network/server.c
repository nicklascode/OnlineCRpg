#include "server.h"
#include "packet.h"
#include "network.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string.h>

#define MAX_CLIENTS 32

typedef struct {
    SOCKET socket;
    SOCKADDR_IN address;
    u8 clientId;
    int connected;
} ClientSocket;

Server server;
ClientSocket clients[MAX_CLIENTS];

void server_init() {
    // Init Winsock
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Create socket
    server.socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    server.address.sin_family = AF_INET;
    server.address.sin_addr.s_addr = INADDR_ANY;
    server.address.sin_port = htons(PORT);

    u_long mode = 1;
    ioctlsocket(server.socket, FIONBIO, &mode);

    // Bind socket
    bind(server.socket, (SOCKADDR*)&server.address, sizeof(server.address));
    listen(server.socket, SOMAXCONN);

    // Initialize client array
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        clients[i].socket = INVALID_SOCKET;
        clients[i].connected = 0;
        clients[i].clientId = i;
    }

    DEBUG_LOG("Server initialized and listening on port %d", PORT);
}

void server_handle_packets() {
    // Accept new clients
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (!clients[i].connected) {
            int addrSize = sizeof(clients[i].address);
            SOCKET newSocket = accept(server.socket, (SOCKADDR*)&clients[i].address, &addrSize);
            if (newSocket != INVALID_SOCKET) {
                clients[i].socket = newSocket;
                u_long mode = 1;
                ioctlsocket(clients[i].socket, FIONBIO, &mode);
                clients[i].connected = 1;
                DEBUG_LOG("Accepted new client connection (clientId=%d)", clients[i].clientId);
            }
        }
    }
    
// Handle packets for each connected client
 #pragma region Packets 
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i].connected && clients[i].socket != INVALID_SOCKET) {
            Packet packet;
            int received = 1;
            while (1) {
                received = receive_packet(clients[i].socket, &packet);
                if (received == 1) {
                    switch (packet.id) {
                        case Greeting: 
                        {
                            PlayerDataPacket greeting;
                            memcpy(&greeting, packet.buffer, sizeof(PlayerDataPacket));
                            DEBUG_LOG("Deserialized greeting from %s:", greeting.username);

                            PlayerDataPacket reply;
                            memset(&reply, 0, sizeof(PlayerDataPacket));
                            strcpy(reply.username, "Server");
                            reply.client_id = clients[i].clientId;

                            Packet reply_packet;
                            serialize_packet(&reply, Greeting, &reply_packet, sizeof(PlayerDataPacket));
                            send_packet(clients[i].socket, &reply_packet);
                            DEBUG_LOG("Sent greeting reply to clientId=%d", clients[i].clientId);
                            break;
                        }

                        // ERRRRR
                        case 0:
                        default:
                            ERROR_LOG("Received unknown packet ID: %d from clientId=%d", packet.id, clients[i].clientId);
                            break;
                    }
                } else if (received == -1) {
                    // No data available, break loop
                    break;
                } else if (received == 0) {
                    // Disconnected
                    DEBUG_LOG("Client disconnected (clientId=%d)", clients[i].clientId);
                    closesocket(clients[i].socket);
                    clients[i].socket = INVALID_SOCKET;
                    clients[i].connected = 0;
                    break;
                }
            }
        }
    }

    #pragma endregion Packets
}

void server_shutdown() {
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i].socket != INVALID_SOCKET) {
            closesocket(clients[i].socket);
        }
    }
    closesocket(server.socket);
    WSACleanup();
}