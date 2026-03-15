#include "raylib.h"

#include "types.h"
#include "global.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;


    InitWindow(screenWidth, screenHeight, "Simple Raylib Window");

    // Initialize global level
    global.level = *generate_basic_level(10, 10);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        draw_level(&global.level);

        if(IsCursorOnScreen())
        {
            Vector2 mousePos = GetMousePosition();
            Tile* hoveredTile = get_tile(&global.level, mousePos);
            if(hoveredTile)
            {
                DrawRectangleLines(hoveredTile->position.x, hoveredTile->position.y, TILE_SIZE, TILE_SIZE, RED);
                DrawText(TextFormat("Tile pos: (%d, %d)", hoveredTile->position.x, hoveredTile->position.y), 10, 10, 20, WHITE);
            }
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}