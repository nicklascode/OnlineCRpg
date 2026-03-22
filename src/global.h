#ifndef GLOBAL_H // So if not exsit
#define GLOBAL_H // make exsit

#include "level/level.h"
#include "network/global_net.h"

typedef struct global {
    Level level;
    Network network;
} Global;

extern Global global; // Extern allows us to use the global variable in other files without defining it multiple times (we define it in global.c)

#endif