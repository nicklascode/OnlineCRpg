#include "level.h"
#include "tile.h"
#include <raylib.h>
#include <stdlib.h>
#include "../util/math.h"

Level* generate_basic_level(u16 width, u16 height) {
    Level* level = (Level*)malloc(sizeof(Level)); // Basiclly the 'new' keyword
    level->width = width;
    level->height = height;
    level->tiles = (TileData*)malloc(sizeof(TileData) * width * height);

    for (u16 y = 0; y < height; y++) {
        for (u16 x = 0; x < width; x++) {
            TileData* tileData = &level->tiles[y * width + x];
            Tile* tile = &Floor_Tile; // For now, all tiles are floor tiles.

            tileData->position = (Vec2I){ x, y}; // Since we don't have a 'new' keyword, and this is a struct, we just use an expression to set the position. (Vector2I){x, y}

            tileData->tile = *tile; // Set the tile data. 
        }
    }

    return level;
}

void draw_level(Level* level) {
    extern Texture2D g_tile_textures[];
    for (u16 y = 0; y < level->height; y++) {
        for (u16 x = 0; x < level->width; x++) {
            TileData* tileData = &level->tiles[y * level->width + x];

            if(tileData->tile.id == 0) continue; // Skip empty tiles
            int tid = tileData->tile.texture_id;
            DrawTexture(g_tile_textures[tid], tileData->position.x * TILE_SIZE, tileData->position.y * TILE_SIZE, WHITE);
        }
    }
}

TileData* get_tile(Level* level, Vec2 position) {
    Vec2I posI = V2_TO_V2I(position);
    u16 x = posI.x / TILE_SIZE;
    u16 y = posI.y / TILE_SIZE;

    if (x < level->width && y < level->height) {
        return &level->tiles[y * level->width + x];
    }
    return NULL; // Out of bounds
}
