#include "entity.h"
#include "entity_types.h"
#include "../network/nettypes.h"
#include "../network/global_net.h"

#ifndef PLAYER_H
#define PLAYER_H

typedef struct {
    u8 client_id;
    char username[128];
    LivingEntityData livingData;
} PlayerData;

typedef struct {
    u8 player_entity_id;
} Player;

extern Player local_player;

void init_player(Entity* player_entity, PlayerData* player_data);
void init_local_player(Entity* player_entity, PlayerData* player_data);
void handle_localPlayer_input();

#endif // !PLAYER_H