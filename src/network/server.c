#include "server.h"
#include "packet.h"
#include "network.h"

Server server;

void init() {
    // Initialize Winsock
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Create a socket and bind it to the address and port
    server.socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    server.address.sin_family = AF_INET;
    server.address.sin_addr.s_addr = INADDR_ANY;
    server.address.sin_port = htons(PORT);

    // Bind the socket and start listening for incoming connections
    bind(server.socket, (SOCKADDR*)&server.address, sizeof(server.address));
    listen(server.socket, SOMAXCONN);
}

void shutdown() {
    WSACleanup();
}