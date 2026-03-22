#include "assets.h"
#include <raylib.h>
#include <stdlib.h>

Texture2D g_tile_textures[TILE_TEXTURE_COUNT];
Texture2D g_entity_textures[ENTITY_TEXTURE_COUNT];

static void load_tile_textures(void) {
    g_tile_textures[0] = (Texture2D){0};
    g_tile_textures[1] = LoadTexture("assets/textures/tiles/Floor.png");
    g_tile_textures[2] = LoadTexture("assets/textures/tiles/Wall.png");
}

static void load_entity_textures(void) {
    g_entity_textures[0] = LoadTexture("assets/textures/entities/Person.png");
}

void assets_init(void) {
    load_tile_textures();
    load_entity_textures();
}
