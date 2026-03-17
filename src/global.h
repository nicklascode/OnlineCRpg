#ifndef GLOBAL_H // So if not exsit
#define GLOBAL_H // make exsit

#include "level/level.h"

typedef struct network {
    u8 mode;
    int isConnected;
} Network;

typedef struct global {
    Level level;
    Network network;
} Global;

extern Global global; // Extern allows us to use the global variable in other files without defining it multiple times (we define it in global.c)

#endif