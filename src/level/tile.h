#pragma once
#include "../util/math.h"
#include "../types.h"

// FLAGS
#define TILE_FLAG_SOLID = (1 << 0); // Shift by well... 0 lol

#define TILE_SIZE 32

typedef struct tile {
    u16 id;
    u16 flags;
    int texture_id; // Index into global tile texture array
} Tile;

// TILES
extern Tile Empty_Tile;
extern Tile Floor_Tile;
extern Tile Wall_Tile;

