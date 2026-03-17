#include "../types.h"

#ifndef ENTITY_H
#define ENTITY_H

#define MAX_COMPONENTS 12

typedef enum {
    ENTITY_NULL = 0,
    ENTITY_PLAYER = 1
} EntityType;

typedef struct entity {
    bool isActive;
    u8 ID;
    EntityType type;

    
} Entity;


#endif // !ENTITY_H