#include "../types.h"
#include "entity_types.h"
#include "sprite.h"
#include "../util/math.h"

#ifndef ENTITY_H
#define ENTITY_H

#define ENTITY_DEBUG_INFO 1

typedef struct entity {
    bool isActive;
    u32 ID;
    EntityType type;

    Vec2 position;
    Sprite* sprite;
    void* data;
} Entity;

typedef struct entity_manager
{
     Entity* entities;
     u16 entity_count;
} EntityManager;

extern EntityManager* current_entity_manager; // Global

EntityManager* create_entity_manager();
Entity* create_entity(EntityManager* manager, EntityType type, u32 ID);
void* get_entity_data(Entity* entity);
Entity* get_entity_by_id(EntityManager* manager, u32 entity_id);
bool remove_entity(EntityManager* manager, u32 entity_id);

#endif // !ENTITY_H