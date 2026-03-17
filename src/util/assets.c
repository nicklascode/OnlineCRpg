#include "assets.h"
#include <raylib.h>
#include <stdlib.h>

Texture2D g_tile_textures[TILE_TEXTURE_COUNT];

static void load_tile_textures(void) {
    g_tile_textures[0] = (Texture2D){0};
    g_tile_textures[1] = LoadTexture("assets/textures/tiles/ShittyPlanks.png");
}

void assets_init(void) {
    load_tile_textures();
}
