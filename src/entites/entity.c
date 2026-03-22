#include "entity.h"
#include "../types.h"
#include "../util/logger.h"

#include <stdlib.h>

EntityManager* current_entity_manager;

EntityManager* create_entity_manager() {
    EntityManager* manager = (EntityManager*)malloc(sizeof(EntityManager));
    manager->entities = NULL; // Start with empty array
    manager->entity_count = 0;
    return manager;
}

Entity* create_entity(EntityManager* manager, EntityType type, u32 ID) {
    if(!manager) return NULL;
    if (manager->entity_count >= 65535) {
        return NULL; // Max entities reached
    }

    // Resize entities array
    size_t new_count = manager->entity_count + 1;
    Entity* new_entities = (Entity*)realloc(manager->entities, sizeof(Entity) * new_count);
    if (!new_entities) {
        // Allocation failed, do not increment count
        return NULL;
    }
    manager->entities = new_entities;
    manager->entity_count = new_count;

    Entity* entity = &manager->entities[manager->entity_count - 1];
    entity->isActive = true;
    entity->ID = ID == -1 ? manager->entity_count : ID; // Assign ID or use count as fallback
    entity->type = type;
    entity->sprite = 0;
    entity->data = 0;
    entity->position = (Vec2) {0, 0}; // Spawn at 0,0

    return entity;
}

void* get_entity_data(Entity* entity) {
    if (!entity->isActive) {
        return NULL;
    }
   
    return entity->data;
}

Entity* get_entity_by_id(EntityManager* manager, u32 entity_id) {
    for (u16 i = 0; i < manager->entity_count; i++) {
        if (manager->entities[i].ID == entity_id) {
            return &manager->entities[i];
        }
    }
    return NULL; // Not found
}

bool remove_entity(EntityManager* manager, u32 entity_id) {
    for (u16 i = 0; i < manager->entity_count; i++) {
        if (manager->entities[i].ID == entity_id) {
            manager->entities[i].isActive = false;

            // Resize entities array by moving the last entity to the removed spot
            Entity* oldEntity = &manager->entities[i];
            Entity* lastEntity = &manager->entities[manager->entity_count - 1];

            // Make sure it's not the last or the same entity
            if(oldEntity != lastEntity) {
                *oldEntity = *lastEntity; // Move last entity to removed spot
            }
            manager->entity_count--; // Decrease count

            // Resize
            Entity* new_entities = (Entity*)realloc(manager->entities, sizeof(Entity) * manager->entity_count);
            if (new_entities || manager->entity_count == 0) {
                manager->entities = new_entities;
            }
            return true;
        }
    }
    return false; // Entity not found
}
