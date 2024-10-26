// General Includes
#include "raylib.h"
#include "definitions.h"
#include <math.h>
#include <string.h>

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

        for(int i = 0; i < enemy_counter; i++)
            {
            if( detect_sweeper_collision(&tower.sweeper, enemies[i]) )
                {
                despawn_enemy( enemies, &enemy_counter, i );
                }
            }

        EndDrawing();
        frame_counter++;
    }

    // Closure steps
    CloseWindow();
    return 0;
}



















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
    geo.posn.x += (hdist > 0.0f) ? (-1.0f * geo.velocity.x * cosf(theta)) : (geo.velocity.x * cosf(theta));
    geo.posn.y += (vdist > 0.0f) ? (-1.0f * geo.velocity.y * sinf(theta)) : (geo.velocity.y * sinf(theta));
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