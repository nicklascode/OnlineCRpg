#include "../types.h"

#ifndef ENTITY_H
#define ENTITY_H

typedef enum {
    ENTITY_NULL = 0,
    ENTITY_PLAYER = 1
} EntityType;

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

extern EntityManager* current_entity_manager; // Global pointer to the current entity manager, so we can access it from anywhere without passing it around (we define it in entity.c)

EntityManager* create_entity_manager(u16 max_entities);
Entity* create_entity(EntityManager* manager, EntityType type);
void* get_entity_data(Entity* entity);
bool remove_entity(EntityManager* manager, u8 entity_id);

#endif // !ENTITY_H