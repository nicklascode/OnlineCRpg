#pragma once
#define NOGDI
#define NOMINMAX

#include <winsock2.h>
#include <ws2tcpip.h>

#include "global_net.h"
#include "packet.h"

#undef CloseWindow
#undef ShowCursor
#undef Rectangle
#undef DrawTextA

typedef struct client {
    SOCKET socket;
    struct sockaddr_in address;
} Client;

void client_init();
void client_handle_packets();
void client_shutdown();
void client_send_packet(Packet* packet);

extern Client local_client;