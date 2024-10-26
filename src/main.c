// General Includes
#include "definitions.h"

int main(void)
{
    // Initialization steps
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Simple Idle Tower Defense Game");
    SetTargetFPS(FRAMERATE);

    // Local variables
    Tower_info_type tower = {};
    Enemy_type enemies[MAX_ENEMIES] = {};
    int frame_counter = 0;
    int enemy_counter = 0;

    init_tower( &tower );

    // Game loop
    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if( frame_counter >= FRAMERATE
         && enemy_counter < MAX_ENEMIES )
            {
            frame_counter = 0;
            spawn_enemy(&enemies[enemy_counter++], tower.range);
            }
        move_enemies(enemies, enemy_counter);
        draw_enemies(enemies, enemy_counter);
        draw_tower(&tower);

        EndDrawing();
        frame_counter++;
    }

    // Closure steps
    CloseWindow();
    return 0;
}