#include "player.h"
#include "../util/logger.h"
#include <raylib.h>

Player local_player;

void init_player(Entity* player_entity, PlayerData* player_data) {
    player_entity->data = player_data;
    player_entity->isActive = true;
}

void init_local_player(Entity* player_entity, PlayerData* player_data) {
    init_player(player_entity, player_data);
    local_player.player_entity_id = player_entity->ID;
}

void handle_localPlayer_input() {
    Entity* player_entity = get_entity_by_id(current_entity_manager, local_player.player_entity_id);
    if(!player_entity) return;

    int dx = 0, dy = 0;

    if(IsKeyDown(KEY_W))
        dy -= ((PlayerData*)player_entity->data)->livingData.speed;
    if(IsKeyDown(KEY_S))
        dy += ((PlayerData*)player_entity->data)->livingData.speed;
    if(IsKeyDown(KEY_A))
        dx -= ((PlayerData*)player_entity->data)->livingData.speed;
    if(IsKeyDown(KEY_D))
        dx += ((PlayerData*)player_entity->data)->livingData.speed;

   
    player_entity->position.x += dx;
    player_entity->position.y += dy;
}