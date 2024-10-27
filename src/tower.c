#include "definitions.h"

// Initialize tower for game start
void init_tower(Tower_info_type* tower)
{
    tower->health.max_hp            = BASE_HEALTH;
    tower->health.curr_hp           = BASE_HEALTH;
    tower->health.hp_regen          = BASE_REGEN;

    tower->defense.def_pct          = BASE_DEF_PCT;
    tower->defense.def_abs          = BASE_DEF_ABS;

    tower->sweeper.center.x         = s_screen_origin.x;
    tower->sweeper.center.y         = s_screen_origin.y;
    tower->sweeper.theta            = BASE_SWEEPER_ANGLE;
    tower->sweeper.radius           = BASE_SWEEPER_RADIUS;
    tower->sweeper.speed            = BASE_SWEEPER_SPEED;

    tower->critical.chance          = BASE_CRIT_CHANCE;
    tower->critical.factor          = BASE_CRIT_FACTOR;

    tower->multishot.chance         = BASE_MULTISHOT_CHANCE;
    tower->multishot.max_targets    = BASE_MULTISHOT_MAX_TARGETS;

    tower->rapid_fire.chance        = BASE_RAPID_FIRE_CHANCE;
    tower->rapid_fire.duration      = BASE_RAPID_FIRE_DURATION;
    tower->rapid_fire.factor        = BASE_RAPID_FIRE_FACTOR;

    tower->bounce_shot.chance       = BASE_BOUNCE_SHOT_CHANCE;
    tower->bounce_shot.max_targets  = BASE_BOUNCE_SHOT_MAX_TARGETS;
    tower->bounce_shot.radius       = BASE_BOUNCE_SHOT_RADIUS;

    tower->range                    = BASE_RANGE;
    tower->damage                   = BASE_DAMAGE;
    tower->attack_speed             = BASE_ATTACK_SPEED;
    tower->projectile_speed         = BASE_PROJECTILE_SPEED;
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

// Detect enemies that are within the tower's range. The function returns the index of the closest enemy, -1 if none are found.
int detect_enemies_in_range(Tower_info_type *tower, Enemy_type *enemies, int enemy_count)
{
    Rectangle   enemy_bbox;
    Vector2     curr_enemy_posn;
    float       curr_enemy_distance;
    float       closest_enemy_distance;
    int         closest_enemy_index;

    // Assign large value so that any tower in range will be closer
    closest_enemy_distance = INV_FLOAT;
    closest_enemy_index = INV_INDEX;

    for(int i = 0; i < enemy_count; i++)
        {
        // Copy enemy data into placeholder variable
        enemy_bbox.height = enemies[i].geo.height;
        enemy_bbox.width = enemies[i].geo.width;
        enemy_bbox.x = enemies[i].geo.posn.x;
        enemy_bbox.y = enemies[i].geo.posn.y;

        // Store enemy position as a vector to check distance
        curr_enemy_posn.x = enemies[i].geo.posn.x;
        curr_enemy_posn.y = enemies[i].geo.posn.y;
        curr_enemy_distance = calc_distance(s_screen_origin, curr_enemy_posn);

        if( CheckCollisionCircleRec(s_screen_origin, tower->range, enemy_bbox)
         && curr_enemy_distance < closest_enemy_distance)
            {
            closest_enemy_distance = curr_enemy_distance;
            closest_enemy_index = i;
            }
        }

    return closest_enemy_index;
}

// Create a projectile aimed at the enemy passed in and return it as an output parameter
void shoot_projectile(Tower_info_type *tower, Enemy_type enemy, Projectile_info_type *out_projectile)
{
    // Local variables
    float enemy_hdist;
    float enemy_vdist;

    // Initialize projectile speed
    out_projectile->speed = tower->projectile_speed;

    // Determine the angle of the projectile
    enemy_hdist = enemy.geo.posn.x - (float)HCENTER;
    enemy_vdist = enemy.geo.posn.y - (float)VCENTER;
    out_projectile->theta = atanf(enemy_vdist / enemy_hdist);

    // Move the projectile into the quadrant of the screen that it will go -> this helps with calculating its trajectory later
    if( out_projectile->theta > 0.0f )
        {
        out_projectile->posn = (enemy_hdist > 0.0f) ?
                                    (Vector2){(float)HCENTER + 1.0f, (float)VCENTER + 1.0f}      // Bottom-right quadrant
                                    :
                                    (Vector2){(float)HCENTER - 1.0f, (float)VCENTER - 1.0f};     // Top-left quadrant
        }
    else
        {
        out_projectile->posn = (enemy_vdist > 0.0f) ?
                                    (Vector2){(float)HCENTER - 1.0f, (float)VCENTER + 1.0f}     // Bottom-left quadrant
                                    :
                                    (Vector2){(float)HCENTER + 1.0f, (float)VCENTER - 1.0f};    // Top-right quadrant
        }
}

// Draw all projectiles
void draw_projectiles(Projectile_info_type *projectiles, int projectile_count)
{
    for(int i = 0; i < projectile_count; i++)
        {
        DrawCircle((int)projectiles[i].posn.x, (int)projectiles[i].posn.y, BASE_PROJECTILE_RADIUS, BLACK);
        }
}

// Advance all projectiles toward their targetted enemy by 1 unit, depending on speed
// Sadly I don't understand why this works but it does - TODO refactor and comment when better understood
void move_projectiles(Projectile_info_type *projectiles, int projectile_count)
{
    float hdist;
    float vdist;

    for(int i = 0; i < projectile_count; i++)
        {
        hdist = projectiles[i].posn.x - (float)HCENTER;
        vdist = projectiles[i].posn.y - (float)VCENTER;

        projectiles[i].posn.x += (hdist < 0.0f) ? (-1.0f * projectiles[i].speed * cosf( projectiles[i].theta)) : (projectiles[i].speed * cosf( projectiles[i].theta));
        if( projectiles[i].theta > 0.0f )
            {
            projectiles[i].posn.y += (vdist < 0.0f) ? (-1.0f * projectiles[i].speed * sinf( projectiles[i].theta)) : (projectiles[i].speed * sinf( projectiles[i].theta));
            }
        else
            {
            projectiles[i].posn.y -= (vdist < 0.0f) ? (-1.0f * projectiles[i].speed * sinf( projectiles[i].theta)) : (projectiles[i].speed * sinf( projectiles[i].theta));
            }
        }    
}