#include "../types.h"

#ifndef ENTITY_TYPES_H
#define ENTITY_TYPES_H

typedef enum {
    ENTITY_NULL = 0,
    ENTITY_PLAYER = 1
} EntityType;

// DATA
typedef struct {
    u32 health;
    u32 damage;
    u8 speed;
} LivingEntityData;

#endif // ENTITY_TYPES_H