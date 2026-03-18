#include "entity.h"
#include "entity_types.h"

#ifndef PLAYER_H
#define PLAYER_H

typedef struct {
    LivingEntityData livingData;
    u8 player_client_id;
    char username[128];
    
} PlayerData;

#endif // !PLAYER_H