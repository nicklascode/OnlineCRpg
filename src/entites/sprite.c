#include "sprite.h"
#include <stdlib.h>

Sprite* create_sprite(int* sprite_texture_ids, int lenght)
{
    Sprite* sprite = (Sprite*)malloc(sizeof(Sprite)); // Init
    sprite->current_sprite_frame = 0;
    sprite->current_sprite_texture_index = 0;
    sprite->sprite_texture_ids = 0;
    sprite->sprite_textures_count = 0;

    if(!sprite_texture_ids || sprite_texture_ids == NULL || lenght <= 0) return sprite; // Return empty sprite
    sprite->sprite_texture_ids = (int*)malloc(lenght * sizeof(int)); // Allocate to size of input array
    
    // Assign
    for(int i = 0; i < lenght; i++) {
        sprite->sprite_texture_ids[i] = sprite_texture_ids[i];
    }
    sprite->sprite_textures_count = lenght;

    return sprite;
}