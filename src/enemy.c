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
    rect.x = enemies[i].geo.posn.x;
    rect.y = enemies[i].geo.posn.y;
    DrawRectangleRec(rect, MAGENTA);
    }
}

// Spawn an enemy at a random location outside the circle and return its data by reference as a rectangle.
void spawn_enemy(Enemy_type *enemy, float tower_radius)
{
    enemy->geo.height = BASIC_ENEMY_HEIGHT;
    enemy->geo.width = BASIC_ENEMY_WIDTH;
    Vector2 posn;

    // Ensure the enemy does not spawn inside the circle
    do
        {
        posn.x = GetRandomValue(0, SCREEN_WIDTH);
        posn.y = GetRandomValue(0, SCREEN_HEIGHT);
        }
    while( CheckCollisionPointCircle(posn, s_screen_origin, tower_radius + SPAWN_BUFFER) );

    enemy->geo.posn.x = posn.x;
    enemy->geo.posn.y = posn.y;
    enemy->geo.velocity = BASE_ENEMY_VELOCITY;
}

// Advance all enemies toward the origin by 1 unit, depending on speed
void move_enemies(Enemy_type *enemies, int enemy_count)
{
// Local macro for readability    
#define geo enemies[i].geo

//Local variables
float hdist;
float vdist;
float theta;

for( int i = 0; i < enemy_count; i++ )
    {
    // Get the vertical and horizontal distance components between the enemy and the center of the screen
    hdist = geo.posn.x - (float)HCENTER;
    vdist = geo.posn.y - (float)VCENTER;

    // Solve for the angle of approach using the arctangent of the ratio of distance components
    theta = atanf(fabsf(vdist / hdist));
    
    // Increment or decrement the x coordinate using the cosine of the angle, and the y coordinate using the sine.
    geo.posn.x += (hdist > 0.0f) ? (-1.0f * geo.velocity * cosf(theta)) : (geo.velocity * cosf(theta));
    geo.posn.y += (vdist > 0.0f) ? (-1.0f * geo.velocity * sinf(theta)) : (geo.velocity * sinf(theta));
    }

// Remove local macro
#undef geo
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

// Despawn enemy by removing it from the array of enemies
void despawn_enemy( Enemy_type *arr, int *arr_size, int index )
{
    if( *arr_size > 0 )
        {
        memmove( &arr[index], &arr[index + 1], (*arr_size - index - 1) * sizeof(Enemy_type) );
        *arr_size -= 1;
        }
}