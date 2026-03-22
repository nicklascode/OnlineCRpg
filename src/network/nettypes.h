#include "../types.h"

#ifndef NET_TYPES_H
#define NET_TYPES_H


typedef enum : u8 {
    NETWORK_NONE = 0,
    NETWORK_CLIENT,
    NETWORK_SERVER,
    NETWORK_HOST // Client and server (MAIN MODE)
} NetworkMode;

typedef struct client_info 
{
    u8 client_id;
    char username[128];
} Client_Info;


#endif
