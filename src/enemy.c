#include "raylib.h"
#include "definitions.h"
#include <math.h>
#include <string.h>

// Draw all enemies.
void draw_enemies(Enemy_type *enemies, int enemy_count)
{
    Rectangle rect;
    for( int i = 0; i < enemy_count; i++ )
        {
        rect.height = enemies[i].geo.height;
        rect.width = enemies[i].geo.width;

        // Position them based on the center of the square, not the top left corner
        rect.x = enemies[i].geo.posn.x - BASIC_ENEMY_WIDTH / 2.0f;
        rect.y = enemies[i].geo.posn.y - BASIC_ENEMY_HEIGHT / 2.0f;
        DrawRectangleRec(rect, MAGENTA);
        }
}

// Spawn an enemy at a random location outside the circle and return its data by reference as a rectangle.
void spawn_enemy(float tower_radius, Enemy_type *out_enemy)
{
    out_enemy->geo.height = BASIC_ENEMY_HEIGHT;
    out_enemy->geo.width = BASIC_ENEMY_WIDTH;
    Vector2 posn;

    // Ensure the enemy does not spawn inside the circle
    // For simplicity with movement functions, ensure they are not directly in the
    // horizontal or vertical center of the screen either.
    do
        {
        posn.x = GetRandomValue(0, SCREEN_WIDTH);
        posn.y = GetRandomValue(0, SCREEN_HEIGHT);
        }
    while( CheckCollisionPointCircle(posn, s_screen_origin, tower_radius + SPAWN_BUFFER)
        || posn.x == (float)HCENTER
        || posn.y == (float)VCENTER );

    // Assign the randomly generated position
    out_enemy->geo.posn.x = posn.x;
    out_enemy->geo.posn.y = posn.y;
    out_enemy->geo.speed = BASE_ENEMY_SPEED;

    // Assign a random ID - hopefully there's never a collision
    out_enemy->id = GetRandomValue(0, __INT32_MAX__);
}

// Advance all enemies toward the origin by 1 unit, depending on speed
void move_enemies(Enemy_type *enemies, int enemy_count)
{
    // Local variables
    float hdist;
    float vdist;
    float theta;

    for( int i = 0; i < enemy_count; i++ )
        {
        // Get the vertical and horizontal distance components between the enemy and the center of the screen
        hdist = enemies[i].geo.posn.x - (float)HCENTER;
        vdist = enemies[i].geo.posn.y - (float)VCENTER;
        theta = calc_enemy_approach_angle(enemies[i]);
        
        // Increment or decrement the x coordinate using the cosine of the angle, and the y coordinate using the sine.
        enemies[i].geo.posn.x += (hdist > 0.0f) ? (-1.0f * enemies[i].geo.speed * cosf(theta)) : (enemies[i].geo.speed * cosf(theta));
        enemies[i].geo.posn.y += (vdist > 0.0f) ? (-1.0f * enemies[i].geo.speed * sinf(theta)) : (enemies[i].geo.speed * sinf(theta));
        }
}

// Retrieve the set of four lines the describe a basic enemy's bounding box in the form of two output arrays.
// The output arrays are assumed to have a minimum size of 4.
void get_basic_enemy_lines(Enemy_geometry_type geo, Vector2 *out_start_points, Vector2 *out_end_points)
{
    // Obtain the start and end points for the top line -> from left to right
    out_start_points[TOP_LINE].x = geo.posn.x;
    out_start_points[TOP_LINE].y = geo.posn.y;
    out_end_points[TOP_LINE].x = geo.posn.x + geo.width;
    out_end_points[TOP_LINE].y = geo.posn.y;
    
    // Obtain the start and end points for the right line -> from top to bottom 
    out_start_points[RIGHT_LINE].x = geo.posn.x + geo.width;
    out_start_points[RIGHT_LINE].y = geo.posn.y;
    out_end_points[RIGHT_LINE].x = geo.posn.x + geo.width;
    out_end_points[RIGHT_LINE].y = geo.posn.y + geo.height;
    
    // Obtain the start and end points for the bottom line -> from left to right
    out_start_points[BOTTOM_LINE].x = geo.posn.x;
    out_start_points[BOTTOM_LINE].y = geo.posn.y + geo.height;
    out_end_points[BOTTOM_LINE].x = geo.posn.x + geo.width;
    out_end_points[BOTTOM_LINE].y = geo.posn.y + geo.height;
    
    // Obtain the start and end points for the left line -> from top to bottom 
    out_start_points[LEFT_LINE].x = geo.posn.x;
    out_start_points[LEFT_LINE].y = geo.posn.y;
    out_end_points[LEFT_LINE].x = geo.posn.x;
    out_end_points[LEFT_LINE].y = geo.posn.y + geo.height;
}