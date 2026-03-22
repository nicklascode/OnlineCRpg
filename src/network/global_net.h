#ifndef GLOBAL_NET_H
#define GLOBAL_NET_H

#include "../network/nettypes.h"

#define MAX_CLIENTS 32

typedef struct network {
    NetworkMode mode;
    int isConnected;
    int isHost;
    int local_client_id;
} Network;

extern Network global_network;

#endif // GLOBAL_NET_H
