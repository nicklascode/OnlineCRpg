#include "level.h"
#include <raylib.h>
#include <stdlib.h>
#include "../util/math.h"

Level* generate_basic_level(u16 width, u16 height) {
    Level* level = (Level*)malloc(sizeof(Level)); // Basiclly the 'new' keyword
    level->width = width;
    level->height = height;
    level->tiles = (Tile*)malloc(sizeof(Tile) * width * height);

    for (u16 y = 0; y < height; y++) {
        for (u16 x = 0; x < width; x++) {
            Tile* tile = &level->tiles[y * width + x];
            tile->id = 0; // Default tile ID
            tile->flags = 0; // Default flags
            tile->position = (Vector2I){ x, y}; // Since we don't have a new keyword, and this is a struct, we just use an expression to set the position. (Vector2I){x, y}
        }
    }

    return level;
}

void draw_level(Level* level) {
    for (u16 y = 0; y < level->height; y++) {
        for (u16 x = 0; x < level->width; x++) {
            Tile* tile = &level->tiles[y * level->width + x];
            DrawRectangle(tile->position.x * TILE_SIZE, tile->position.y * TILE_SIZE, TILE_SIZE, TILE_SIZE, GRAY);
        }
    }
}

Tile* get_tile(Level* level, Vector2 position) {
    Vector2I posI = V2_TO_V2I(position);
    u16 x = posI.x / TILE_SIZE;
    u16 y = posI.y / TILE_SIZE;

    if (x < level->width && y < level->height) {
        return &level->tiles[y * level->width + x];
    }
    return NULL; // Out of bounds
}
