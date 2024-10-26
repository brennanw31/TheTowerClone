#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

// Game state values
#define SCREEN_WIDTH    ( 1280 )
#define SCREEN_HEIGHT   ( 720 )
#define FRAMERATE       ( 240 )

// Coordinate helpers
#define HCENTER         ( SCREEN_WIDTH / 2 )
#define VCENTER         ( SCREEN_HEIGHT / 2 )

#define SPAWN_BUFFER    ( 150.0f )
#define MAX_ENEMIES     ( 100 )

// Enemy dimensions
#define BASIC_ENEMY_HEIGHT  ( 20.0f )
#define BASIC_ENEMY_WIDTH   ( 20.0f )

// Literal constants
#define TOWER_TOP_POINT     (Vector2){ (float)HCENTER, (float)VCENTER + 50.0f }
#define TOWER_RIGHT_POINT   (Vector2){ (float)HCENTER + 57.735f, (float)VCENTER - 50.0f }
#define TOWER_LEFT_POINT    (Vector2){ (float)HCENTER - 57.735f, (float)VCENTER - 50.0f }

// Enumerations
typedef enum {
    ENEMY_VAR_BASIC,
    ENEMY_VAR_TANK,
    ENEMY_VAR_RANGED,
    ENEMY_VAR_BOSS,
    
    ENEMY_VAR_COUNT
    }Enemy_variant_enum;

enum{
    TOP_LINE,
    RIGHT_LINE,
    BOTTOM_LINE,
    LEFT_LINE,
    ENEMY_BBOX_LINE_COUNT
    };

// Types
typedef struct {
    int     number;
    float   spawn_rate;
    }Wave_info_type;

// Enemy types
typedef struct {
    float       height;
    float       width;
    Vector2     posn;
    Vector2     velocity;
    }Enemy_geometry_type;

typedef struct {
    float                   max_health;
    float                   health;
    float                   damage;
    }Enemy_stats_type;

typedef struct {
    Enemy_geometry_type     geo;
    Enemy_stats_type        stats;
    Enemy_variant_enum      variant;
    }Enemy_type;

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
    }Tower_info_type;


// Memory Constants
static const Vector2 s_screen_origin = { HCENTER, VCENTER };

// Enemy function definitions
void move_enemies(Enemy_type *enemies, int enemy_count);
void spawn_enemy(Enemy_type *enemy, float tower_radius);
void get_basic_enemy_lines(Enemy_geometry_type geo, Vector2 *out_start_points, Vector2 *out_end_points);
void draw_enemies(Enemy_type *enemies, int enemy_count);
void despawn_enemy( Enemy_type *arr, int *arr_size, int index );

// Tower function definitions
void draw_tower(Tower_info_type* tower);
void draw_sweeper(Sweeper_info_type* sweeper);
bool detect_sweeper_collision(Sweeper_info_type* sweeper, Enemy_type enemy);

#endif