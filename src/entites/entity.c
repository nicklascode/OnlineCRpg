#include "entity.h"
#include "../types.h"

#include <stdlib.h>

EntityManager* current_entity_manager;

EntityManager* create_entity_manager(u16 max_entities) {
    EntityManager* manager = (EntityManager*)malloc(sizeof(EntityManager));
    manager->entities = (Entity*)malloc(sizeof(Entity) * max_entities);
    manager->entity_count = 0;
    current_entity_manager = manager;
    return manager;
}

Entity* create_entity(EntityManager* manager, EntityType type) {
    if (manager->entity_count >= 65535) {
        return NULL; // Max entities reached
    }

    // Resize entities array
    manager->entity_count++;
    realloc(manager->entities, sizeof(Entity) * manager->entity_count);

    Entity* entity = &manager->entities[manager->entity_count - 1];
    entity->isActive = true;
    entity->ID = manager->entity_count;
    entity->type = type;

    return entity;
}

void* get_entity_data(Entity* entity) {
    if (!entity->isActive) {
        return NULL;
    }
   
    return entity->data;
}

bool remove_entity(EntityManager* manager, u8 entity_id) {
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
            realloc(manager->entities, sizeof(Entity) * manager->entity_count);
            return true;
        }
    }
    return false; // Entity not found
}
