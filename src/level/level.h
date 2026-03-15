#include "../types.h"
#include "../util/math.h"

#define TILE_SIZE 32

typedef struct tile {
    u16 id;
    u16 flags;
    Vector2I position;
} Tile;

typedef struct level {
    u16 width;
    u16 height;
    Tile* tiles;
} Level;

Level* generate_basic_level(u16 width, u16 height);
void draw_level(Level* level);
Tile* get_tile(Level* level, Vector2 position);