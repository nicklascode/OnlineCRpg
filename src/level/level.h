#include "../types.h"
#include "../util/math.h"
#include "tile.h"

typedef struct tile_data 
{
    Tile tile;
    Vec2I position;
} TileData;

typedef struct level {
    u16 width;
    u16 height;
    TileData* tiles;
} Level;

Level* generate_basic_level(u16 width, u16 height);
void draw_level(Level* level);
TileData* get_tile(Level* level, Vec2 position);