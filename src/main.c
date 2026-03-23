#include "types.h"
#include "global.h"
#include "util/logger.h"
#include "raylib.h"

#include  "network/nettypes.h"

#include "util/assets.h"
#include "entites/entity.h"
#include "entites/entity_system.h"
#include "entites/sprite.h"
#include "entites/player.h"


int main(int argc, char** argv)
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    if (argc > 1) {

        if (strcmp(argv[1], "Server") == 0) {
            network_init(NETWORK_SERVER);
        } else if (strcmp(argv[1], "Client") == 0) {
            network_init(NETWORK_CLIENT);
        } else if (strcmp(argv[1], "Host") == 0) {
            network_init(NETWORK_HOST);
        } else {
            ERROR_LOG("Invalid argument. Use 'Server', 'Client', or 'Host'.");
            return -1;
        }
    }

    InitWindow(screenWidth, screenHeight, "Need name plz");
    assets_init();

    current_entity_manager = create_entity_manager();

    DEBUG_LOG("Entity manager created!");
    
    Entity* player_entity = create_entity(current_entity_manager, ENTITY_PLAYER, -1);
    if (player_entity) {
        player_entity->position = (Vec2){100, 100};
        int player_texture_ids[] = {0}; // Assuming texture ID 0 is valid
        player_entity->sprite = create_sprite(player_texture_ids, 1);

        PlayerData player_data = {
            .client_id = 0,
            .username = "Player1",
            .livingData = {
                .health = 100,
                .damage = 10,
                .speed = 1
            }
        };
        init_player(player_entity, &player_data);

    }

    DEBUG_LOG("Created %d entities", current_entity_manager->entity_count);

    // Initialize global level
    global.level = *generate_basic_level(10, 10);

    DEBUG_LOG("Game started with level size: %d x %d", global.level.width, global.level.height);

    int frame = 0;
    const double tick_rate = 1.0 / 60.0; // 60 TPS
    double tick_accumulator = 0.0;
    double last_time = GetTime();

    while (!WindowShouldClose())
    {
        double current_time = GetTime();
        double delta_time = current_time - last_time;
        last_time = current_time;
        tick_accumulator += delta_time;

        // Tick loop
        while (tick_accumulator >= tick_rate)
        {
            network_update();
            update_entities(current_entity_manager);
            tick_accumulator -= tick_rate;
        }

        // Drawing
        BeginDrawing();
        ClearBackground(BLACK);
        draw_level(&global.level);
        draw_entities(current_entity_manager);

        if(IsCursorOnScreen())
        {
            Vec2 mousePos = {GetMousePosition().x, GetMousePosition().y};
            TileData* hoveredTile = get_tile(&global.level, mousePos);
            if (hoveredTile) {
                DrawRectangleLines(hoveredTile->position.x * TILE_SIZE, hoveredTile->position.y * TILE_SIZE, TILE_SIZE, TILE_SIZE, RED);
                DrawText(TextFormat("Tile X: %d, Y: %d", hoveredTile->position.x, hoveredTile->position.y), 10, 10, 20, WHITE);
            }

            if(IsMouseButtonPressed(0)) {
                Entity* _entity = create_entity(current_entity_manager, ENTITY_PLAYER, -1);
                if (_entity) {
                    _entity->position = (Vec2){mousePos.x, mousePos.y};
                    _entity->sprite = create_sprite(0,0);
                }
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