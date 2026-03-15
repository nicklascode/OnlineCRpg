#include <winsock2.h>
#include "packet.h"

typedef struct server {
    SOCKET socket;
    struct sockaddr_in address;
} Server;

void init();
void shutdown();