#include "../types.h"

#ifndef NET_TYPES_H
#define NET_TYPES_H


typedef enum : u8 {
    NETWORK_NONE = 0,
    NETWORK_CLIENT,
    NETWORK_SERVER
} NetworkMode;

#endif
