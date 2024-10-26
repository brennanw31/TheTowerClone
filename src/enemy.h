#ifndef ENEMY_H_
#define ENEMY_H_

#include "definitions.h"

#define SPAWN_BUFFER    ( 25.0f )
#define MAX_ENEMIES     ( 25 )

// Enemy dimensions
#define BASIC_ENEMY_HEIGHT  ( 20.0f )
#define BASIC_ENEMY_WIDTH   ( 20.0f )

#define BASE_ENEMY_VELOCITY ( 1.0f )

// Enumerations
typedef enum {
    ENEMY_VAR_BASIC,
    ENEMY_VAR_TANK,
    ENEMY_VAR_RANGED,
    ENEMY_VAR_BOSS,
    
    ENEMY_VAR_COUNT
    }Enemy_variant_enum;

// Enemy types
typedef struct {
    float       height;
    float       width;
    float       velocity;
    Vector2     posn;
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

// Enemy function definitions
void move_enemies(Enemy_type *enemies, int enemy_count);
void spawn_enemy(Enemy_type *enemy, float tower_radius);
void get_basic_enemy_lines(Enemy_geometry_type geo, Vector2 *out_start_points, Vector2 *out_end_points);
void draw_enemies(Enemy_type *enemies, int enemy_count);
void despawn_enemy( Enemy_type *arr, int *arr_size, int index );


#endif