#include "types.h"
#include "global.h"
#include "util/logger.h"
#include "raylib.h"

#include  "network/nettypes.h"

#include "util/assets.h"
#include "entites/entity.h"


int main(int argc, char** argv)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    if (argc > 1) {

        if (strcmp(argv[1], "Server") == 0) {
            network_init(NETWORK_SERVER);
        } else if (strcmp(argv[1], "Client") == 0) {
            network_init(NETWORK_CLIENT);
        }
    }

    InitWindow(screenWidth, screenHeight, "Simple Raylib Window");
    assets_init();

    // Initialize global level
    global.level = *generate_basic_level(10, 10);

    DEBUG_LOG("Game started with level size: %d x %d", global.level.width, global.level.height);

    int frame = 0;
    while (!WindowShouldClose())
    {
        network_update();

        BeginDrawing();
        ClearBackground(BLACK);
        draw_level(&global.level);

        if(IsCursorOnScreen())
        {
            Vec2 mousePos = {GetMousePosition().x, GetMousePosition().y};
            TileData* hoveredTile = get_tile(&global.level, mousePos);
            if (hoveredTile) {
                DrawRectangleLines(hoveredTile->position.x * TILE_SIZE, hoveredTile->position.y * TILE_SIZE, TILE_SIZE, TILE_SIZE, RED);
                DrawText(TextFormat("Tile X: %d, Y: %d", hoveredTile->position.x, hoveredTile->position.y), 10, 10, 20, WHITE);
            }
        }

        EndDrawing();
    }

    DEBUG_LOG("Main loop exited, shutting down network and closing window");
    network_shutdown();
    CloseWindow();
    DEBUG_LOG("Client exited cleanly");
    return 0;
}