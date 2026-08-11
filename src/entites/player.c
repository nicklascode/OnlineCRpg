#include "player.h"
#include "../util/logger.h"
#include "../network/client.h"
#include "../network/packet.h"
#include "../raylib_m.h"

Player local_player;
long last_move_packet_time = 0; // Timestamp of the last sent move packet

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

    if(dx != 0 || dy != 0 && GetTime() * 1000 - last_move_packet_time > 300) { // Send packet every 300ms
        // Send movement packet to server
        Packet move_packet;
        MoveEntityPacket move_data;
        move_data.entity_id = player_entity->ID;
        move_data.new_x = player_entity->position.x;
        move_data.new_y = player_entity->position.y;

        serialize_packet(&move_data, C_MoveEntity, &move_packet, sizeof(MoveEntityPacket));
        client_send_packet(&move_packet);
        last_move_packet_time = GetTime() * 1000; // Ms
    }
}