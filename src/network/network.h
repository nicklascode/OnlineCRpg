#include "../types.h"
#include "nettypes.h"

#include <winsock2.h>
#include <ws2tcpip.h>

#define PORT 7020

void network_init(NetworkMode mode);
void network_update();
void network_shutdown();
