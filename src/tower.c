#include "raylib.h"
#include "definitions.h"
#include <math.h>
#include <string.h>

// Draw the main tower to be defended.
void draw_tower(Tower_info_type* tower)
{
    // Draw the tower's outer radius
    DrawRing(s_screen_origin, tower->range - 10.0f, tower->range, 0.0f, 360.0f, 36, LIGHTGRAY);

    // Draw the main tower
    DrawTriangle(TOWER_TOP_POINT, TOWER_LEFT_POINT, TOWER_RIGHT_POINT, RED);
}

// Draw a sweeper -> a line that swings around a point clockwise
void draw_sweeper(Sweeper_info_type* sweeper)
{
}

// Detect when the sweeping line collides with an enemy
bool detect_sweeper_collision(Sweeper_info_type* sweeper, Enemy_type enemy)
{
Vector2 enemy_bbox_start_points[ENEMY_BBOX_LINE_COUNT];
Vector2 enemy_bbox_end_points[ENEMY_BBOX_LINE_COUNT];
Vector2 sweeper_end_point;
Vector2 collision_point;

sweeper_end_point.x = sweeper->radius * cosf(sweeper->theta);
sweeper_end_point.y = sweeper->radius * cosf(sweeper->theta);

get_basic_enemy_lines(enemy.geo, enemy_bbox_start_points, enemy_bbox_end_points);

for(int i = 0; i < ENEMY_BBOX_LINE_COUNT; i++)
    {
    if( CheckCollisionLines(s_screen_origin, sweeper_end_point, enemy_bbox_start_points[i], enemy_bbox_end_points[i], &collision_point) )
        {
        return true;
        }
    }
return false;
}