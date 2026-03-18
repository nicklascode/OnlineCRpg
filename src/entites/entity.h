#include "../types.h"
#include "entity_types.h"

#ifndef ENTITY_H
#define ENTITY_H

typedef struct entity {
    bool isActive;
    u8 ID;
    EntityType type;

    void* data;
} Entity;

typedef struct entity_manager
{
     Entity* entities;
     u16 entity_count;
} EntityManager;

extern EntityManager* current_entity_manager; // Global

EntityManager* create_entity_manager();
Entity* create_entity(EntityManager* manager, EntityType type);
void* get_entity_data(Entity* entity);
bool remove_entity(EntityManager* manager, u8 entity_id);

#endif // !ENTITY_H