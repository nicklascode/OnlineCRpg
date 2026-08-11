#include "entity_system.h"
#include "../raylib_m.h"
#include "../util/assets.h"
#include "player.h"
#include "entity.h"

void draw_entities(EntityManager* manager) {
    if (!manager) return;

    for(int i = 0; i < manager->entity_count; i++) {
        Entity* entity = &manager->entities[i];
        if(!entity) continue;

        if(entity->isActive && entity->type != ENTITY_NULL) {
            if(entity->sprite == NULL) {
                continue;
            }
            if(entity->sprite->sprite_texture_ids == NULL) {
                DrawRectangle(entity->position.x, entity->position.y, 32, 32, RED);
                continue;
            }

            // Debug
            if(ENTITY_DEBUG_INFO) {
                DrawText(TextFormat("ID: %d", entity->ID), entity->position.x, entity->position.y - 20, 10, GREEN);
                DrawText(TextFormat("Type: %d", entity->type), entity->position.x, entity->position.y - 10, 10, GREEN);
                DrawText(TextFormat("Sprite Textures: %d", entity->sprite->sprite_textures_count), entity->position.x, entity->position.y, 10, GREEN);
            }

            Texture2D texture = g_entity_textures[entity->sprite->sprite_texture_ids[entity->sprite->current_sprite_texture_index]];
            if(texture.id != 0) { // Check if texture is valid
                DrawTexture(texture, entity->position.x, entity->position.y, WHITE);
            } else {
                // If texture is invalid, draw placeholder
                DEBUG_LOG("Entity %d has invalid texture", entity->ID);
                DrawRectangle(entity->position.x, entity->position.y, 32, 32, RED);
            }
        }
    }
}

void update_entities(EntityManager* manager) {
    if (!manager) return;

    for(int i = 0; i < manager->entity_count; i++) {
        Entity* entity = &manager->entities[i];
        if(!entity) continue;

        if(entity->isActive && entity->type != ENTITY_NULL) {
            if(entity->data == NULL) {
                continue;
            }

            switch (entity->type)
            {
                case ENTITY_PLAYER:
                    handle_localPlayer_input();
                    break;
                default:
                    break;
            }
        }
    }
}