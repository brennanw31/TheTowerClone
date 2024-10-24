// General Includes
#include "raylib.h"
#include "definitions.h"
#include<math.h>
#include<string.h>

// Memory Constants
static const Vector2 origin = {HCENTER, VCENTER};

int main(void)
{
    // Initialization steps
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Simple Idle Tower Defense Game");
    SetTargetFPS(FRAMERATE);

    // Local variables
    Rectangle enemies[MAX_ENEMIES] = {};
    Vector2 posn = {HCENTER, VCENTER - CIRCLE_RADIUS};
    int frame_counter = potato();
    int enemy_counter = 0;
    float enemy_speed = 2.0f;
    float tower_speed = 9.0f;

    // Game loop
    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if( frame_counter >= FRAMERATE / 5
         && enemy_counter < MAX_ENEMIES )
            {
            frame_counter = 0;
            spawn_basic_enemy(&enemies[enemy_counter++]);
            }
        move_enemies(enemies, enemy_counter, enemy_speed);
        draw_enemies(enemies, enemy_counter);
        draw_tower(&posn, tower_speed);

        for(int i = 0; i < enemy_counter; i++)
            {
            if( detect_collisions(posn, enemies[i]) )
                {
                rect_array_remove( enemies, &enemy_counter, i );
                }
            }

        EndDrawing();
        frame_counter++;
    }

    // Closure steps
    CloseWindow();
    return 0;
}

// Detect when the sweeping line collides with an enemy
bool detect_collisions(Vector2 sweeper_posn, Rectangle enemy)
{
Vector2 enemy_bbox_start_points[ENEMY_BBOX_LINE_COUNT];
Vector2 enemy_bbox_end_points[ENEMY_BBOX_LINE_COUNT];
Vector2 collision_point;

get_basic_enemy_lines(enemy, enemy_bbox_start_points, enemy_bbox_end_points);

for(int i = 0; i < ENEMY_BBOX_LINE_COUNT; i++)
    {
    if( CheckCollisionLines(origin, sweeper_posn, enemy_bbox_start_points[i], enemy_bbox_end_points[i], &collision_point) )
        {
        return true;
        }
    }
return false;
}

// Advance all enemies toward the origin by 1 unit, depending on speed
void move_enemies(Rectangle *enemies, int enemy_count, int speed)
{
// Don't fucken touch this
for( int i = 0; i < enemy_count; i++ )
    {
    float hdist = enemies[i].x - (float)HCENTER;
    float vdist = (float)VCENTER - enemies[i].y;
    float theta = atanf(-1.0f*fabsf(vdist / hdist));
    
    enemies[i].x += (hdist > 0.0f) ? (-1.0f * speed * cosf(theta)) : (speed * cosf(theta));
    enemies[i].y += (vdist > 0.0f) ? (-1.0f * speed * sinf(theta)) : (speed * sinf(theta));
    }
}

// Retrieve the set of four lines the describe a basic enemy's bounding box in the form of two output arrays.
// The output arrays are assumed to have a minimum size of 4.
void get_basic_enemy_lines(Rectangle enemy, Vector2 *out_start_points, Vector2 *out_end_points)
{
    // Obtain the start and end points for the top line -> from left to right
    out_start_points[TOP_LINE].x = enemy.x;
    out_start_points[TOP_LINE].y = enemy.y;
    out_end_points[TOP_LINE].x = enemy.x + enemy.width;
    out_end_points[TOP_LINE].y = enemy.y;
    
    // Obtain the start and end points for the right line -> from top to bottom 
    out_start_points[RIGHT_LINE].x = enemy.x + enemy.width;
    out_start_points[RIGHT_LINE].y = enemy.y;
    out_end_points[RIGHT_LINE].x = enemy.x + enemy.width;
    out_end_points[RIGHT_LINE].y = enemy.y + enemy.height;
    
    // Obtain the start and end points for the bottom line -> from left to right
    out_start_points[BOTTOM_LINE].x = enemy.x;
    out_start_points[BOTTOM_LINE].y = enemy.y + enemy.height;
    out_end_points[BOTTOM_LINE].x = enemy.x + enemy.width;
    out_end_points[BOTTOM_LINE].y = enemy.y + enemy.height;
    
    // Obtain the start and end points for the left line -> from top to bottom 
    out_start_points[LEFT_LINE].x = enemy.x;
    out_start_points[LEFT_LINE].y = enemy.y;
    out_end_points[LEFT_LINE].x = enemy.x;
    out_end_points[LEFT_LINE].y = enemy.y + enemy.height;
}

// Spawn an enemy at a random location outside the circle and return its data by reference as a rectangle.
void spawn_basic_enemy(Rectangle *enemy)
{
    enemy->height = BASIC_ENEMY_HEIGHT;
    enemy->width = BASIC_ENEMY_WIDTH;
    Vector2 posn;

    // Ensure the enemy does not spawn inside the circle
    do
        {
        posn.x = GetRandomValue(0, SCREEN_WIDTH);
        posn.y = GetRandomValue(0, SCREEN_HEIGHT);
        }
    while( CheckCollisionPointCircle(posn, origin, CIRCLE_RADIUS + SPAWN_BUFFER) );

    enemy->x = posn.x;
    enemy->y = posn.y;
}

// Draw all enemies.
void draw_enemies( Rectangle *enemies, int enemy_count)
{
for( int i = 0; i < enemy_count; i++ )
    {
    DrawRectangleRec(enemies[i], MAGENTA);
    }
}

// Draw the main tower to be defended.
void draw_tower(Vector2 *posn, float speed)
{
    // Draw a circle with a radius line moving around it clockwise
    float hdist = posn->x - (float)HCENTER;
    float vdist = (float)VCENTER - posn->y;

    // Deduce which quartile of the circle the sweeper is in
    Quartile_type quartile;
    if( fabsf(hdist) > fabsf(vdist) )
        {
        quartile = (hdist > 0.0f) ? RIGHT_QUARTILE : LEFT_QUARTILE;
        }
    else
        {
        quartile = (vdist > 0.0f) ? UPPER_QUARTILE : LOWER_QUARTILE;
        }

    // Achieve the motion by incrementing one coordinate and then adjusting the other according to the formula for a circle:      x^2 + y^2 = r^2
    switch( quartile % 4 )
        {
        // Increment x and adjust y when in Upper Quartile
        case UPPER_QUARTILE:
            posn->x += speed;
            hdist += speed;
            posn->y = (float)VCENTER - sqrtf(fabsf(powf(CIRCLE_RADIUS, 2.0f) - powf(hdist, 2.0f )));
            break;

        // Increment y and adjust x when in Right Quartile
        case RIGHT_QUARTILE:
            posn->y += speed;
            vdist += speed;
            posn->x = (float)HCENTER + sqrtf(fabsf(powf(CIRCLE_RADIUS, 2.0f) - powf(vdist, 2.0f )));
            break;

        // Decrement x and adjust y when in Lower Quartile
        case LOWER_QUARTILE:
            posn->x -= speed;
            hdist -= speed;
            posn->y = (float)VCENTER + sqrtf(fabsf(powf(CIRCLE_RADIUS, 2.0f) - powf(hdist, 2.0f )));
            break;

        // Decrement y and adjust x when in Left Quartile
        case LEFT_QUARTILE:
            posn->y -= speed;
            vdist -= speed;
            posn->x = (float)HCENTER - sqrtf(fabsf(powf(CIRCLE_RADIUS, 2.0f) - powf(vdist, 2.0f )));
            break;
        }

    // Draw the sweeping line
    DrawLineEx(origin, *posn, 3.0f, BLACK);

    // Draw the small circle at the origin of the line
    DrawCircleV(origin, 15.0f, BLACK);

    // Draw the thin outer ring
    DrawRing(origin, CIRCLE_RADIUS - 1.5f, CIRCLE_RADIUS + 1.5f, 0.0f, 360.0f, 36, BLACK);
}

// Remove an item from an array of rectangles
void rect_array_remove( Rectangle *arr, int *arr_size, int index )
{
    if( *arr_size > 0 )
        {
        memmove( &arr[index], &arr[index + 1], (*arr_size - index - 1) * sizeof(Rectangle) );
        *arr_size -= 1;
        }
} 