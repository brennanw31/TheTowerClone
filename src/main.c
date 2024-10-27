// General Includes
#include "definitions.h"

int main(void)
{
    // Initialization steps
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Simple Idle Tower Defense Game");
    SetTargetFPS(FRAMERATE);

    // Local variables
    Tower_info_type             tower = {};
    Enemy_type                  enemies[MAX_ENEMIES] = {};
    Projectile_info_type        projectiles[MAX_POJECTILES] = {};
    int enemy_spawn_timer = 0;
    int projectile_timer = 0;
    int enemy_counter = 0;
    int projectile_counter = 0;
    int close_enemy_index = INV_INDEX;

    init_tower( &tower );

    // Game loop
    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Check that enough time has passed and there aren't already too many enemies
        if( enemy_spawn_timer >= FRAMERATE
         && enemy_counter < MAX_ENEMIES )
            {
            // Reset timer and spawn enemy
            enemy_spawn_timer = 0;
            spawn_enemy(tower.range, &enemies[enemy_counter++]);
            }
        move_enemies(enemies, enemy_counter);

        // Determine the closest enemy in range, if there is one
        close_enemy_index = detect_enemies_in_range(&tower, enemies, enemy_counter);

        // Check that enough time has passes and there aren't already too many projectiles
        if( projectile_timer >= FRAMERATE
         && projectile_counter < MAX_POJECTILES
         && close_enemy_index != INV_INDEX)
            {
            // Reset timer and shoot projectile
            projectile_timer = 0;
            shoot_projectile(&tower, enemies[close_enemy_index], &projectiles[projectile_counter++]);
            }
        move_projectiles(projectiles, projectile_counter);

        // Draw all screen elements
        draw_enemies(enemies, enemy_counter);
        draw_projectiles(projectiles, projectile_counter);
        draw_tower(&tower);

        EndDrawing();
        enemy_spawn_timer++;
        projectile_timer++;
    }

    // Closure steps
    CloseWindow();
    return 0;
}

// Calculate the distance between two points using the general distance formula:
// distance = sqrt((x2 - x1)^2 + (y2 - y1)^2)
float calc_distance(Vector2 point1, Vector2 point2)
    {
    return sqrtf(powf((point2.x - point1.x), 2.0f) + powf((point2.y - point1.y), 2.0f));
    }

// Calculate the angle from which an enemy is approaching -> they always move toward the center of the screen
// Return value is in radians
float calc_enemy_approach_angle(Enemy_type enemy)
{
    // Solve for the angle of approach using the arctangent of the ratio of distance components
    return atanf(fabsf((enemy.geo.posn.y - (float)VCENTER) / (enemy.geo.posn.x - (float)HCENTER)));
}