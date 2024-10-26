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