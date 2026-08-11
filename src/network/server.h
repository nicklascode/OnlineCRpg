#define NOGDI
#define NOMINMAX
#include <winsock2.h>
#include "packet.h"
#undef CloseWindow
#undef ShowCursor
#undef Rectangle
#undef DrawTextA

typedef struct server {
    SOCKET socket;
    struct sockaddr_in address;
} Server;

void server_init();
void server_handle_packets();
void server_shutdown();
void server_send_packet(int clientId, Packet* packet);