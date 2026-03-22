#include "../types.h"

#ifndef SPRITE_H
#define SPRITE_H

typedef struct entity_sprite
{
    int* sprite_texture_ids; // An array of all the texture ids for this sprite
    u8 sprite_textures_count;
    u8 current_sprite_texture_index;
    u8 current_sprite_frame;
} Sprite;

Sprite* create_sprite(int* sprite_texture_ids, int lenght);

#endif // !SPRITE_H