#ifndef TOWER_H_
#define TOWER_H_

#include "definitions.h"

// Literal constants
#define TOWER_TOP_POINT     (Vector2){ (float)HCENTER, (float)VCENTER - 21.65f }
#define TOWER_RIGHT_POINT   (Vector2){ (float)HCENTER + 25.0f, (float)VCENTER + 21.65f }
#define TOWER_LEFT_POINT    (Vector2){ (float)HCENTER - 25.0f, (float)VCENTER + 21.65f }

#define BASE_HEALTH         ( 10.0f )
#define BASE_REGEN          ( 0.0f )

#define BASE_DEF_PCT        ( 0.0f )
#define BASE_DEF_ABS        ( 0.0f )

#define BASE_SWEEPER_ANGLE      ( 0.0f )
#define BASE_SWEEPER_RADIUS     ( 10.0f )
#define BASE_SWEEPER_SPEED      ( 1.0f )

#define BASE_CRIT_CHANCE        ( 0.0f )
#define BASE_CRIT_FACTOR        ( 1.2f )

#define BASE_MULTISHOT_CHANCE       ( 0.0f )
#define BASE_MULTISHOT_MAX_TARGETS  ( 2.0f )

#define BASE_RAPID_FIRE_CHANCE      ( 0.0f )
#define BASE_RAPID_FIRE_DURATION    ( 0.5f )
#define BASE_RAPID_FIRE_FACTOR      ( 2.0f )

#define BASE_BOUNCE_SHOT_CHANCE         ( 0.0f )
#define BASE_BOUNCE_SHOT_MAX_TARGETS    ( 1.0f )
#define BASE_BOUNCE_SHOT_RADIUS         ( 1.0f )

#define BASE_RANGE              ( 150.0f )
#define BASE_DAMAGE             ( 1.0f )
#define BASE_ATTACK_SPEED       ( 1.0f )
#define BASE_PROJECTILE_SPEED   ( 1.0f )
#define BASE_PROJECTILE_RADIUS  ( 2.5f )

// Tower types
typedef struct {
    float               max_hp;
    float               curr_hp;
    float               hp_regen;
    }Health_info_type;

typedef struct {
    float               def_pct;
    float               def_abs;
    }Defense_info_type;

typedef struct {
    Vector2 center;
    float   theta;
    float   radius;
    float   speed;
    }Sweeper_info_type;

typedef struct {
    float               chance;
    float               factor;
    }Criticals_info_type;

typedef struct {
    float               chance;
    int                 max_targets;
    }Multishot_info_type;

typedef struct {
    float               chance;
    float               factor;
    float               duration;
    }Rapid_fire_info_type;

typedef struct {
    float               chance;
    float               max_targets;
    float               radius;
    }Bounce_shot_info_type;
    
typedef struct {
    Health_info_type        health;
    Defense_info_type       defense;
    Sweeper_info_type       sweeper;
    Criticals_info_type     critical;
    Multishot_info_type     multishot;
    Rapid_fire_info_type    rapid_fire;
    Bounce_shot_info_type   bounce_shot;
    float                   range;
    float                   attack_speed;
    float                   damage;
    float                   projectile_speed;
    }Tower_info_type;

typedef struct {
    Vector2     posn;
    float       theta;
    float       speed;
    }Projectile_info_type;

// Tower function definitions
void init_tower(Tower_info_type* tower);
void draw_tower(Tower_info_type* tower);
void draw_sweeper(Sweeper_info_type* sweeper);
bool detect_sweeper_collision(Sweeper_info_type* sweeper, Enemy_type enemy);
int detect_enemies_in_range(Tower_info_type *tower, Enemy_type *enemies, int enemy_count);
void shoot_projectile(Tower_info_type *tower, Enemy_type enemy, Projectile_info_type *out_projectile);
void move_projectiles(Projectile_info_type *projectiles, int projectile_count);
void draw_projectiles(Projectile_info_type *projectiles, int projectile_count);

#endif