#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

// Global include necesities
#include "raylib.h"
#include "enemy.h"
#include "tower.h"
#include <math.h>
#include <string.h>

// Game state values
#define SCREEN_WIDTH    ( 1280 )
#define SCREEN_HEIGHT   ( 720 )
#define FRAMERATE       ( 240 )

#define SPAWN_BUFFER    ( 25.0f )
#define MAX_ENEMIES     ( 25 )
#define MAX_POJECTILES  ( 50 )

// Coordinate helpers
#define HCENTER         ( SCREEN_WIDTH / 2 )
#define VCENTER         ( SCREEN_HEIGHT / 2 )

// Invalidity
#define INV_FLOAT       ( 9.0E20f )
#define INV_INDEX       ( -1 )

// Macros
#define array_remove( arr, idx )    memmove( &arr[idx], &arr[idx + 1], (sizeof(arr)/sizeof(arr[0]) - idx - 1) * sizeof(arr[0]))

// Enumerations
typedef enum {
    TOP_LEFT_QUADRANT,
    TOP_RIGHT_QUADRANT,
    BOTTOM_RIGHT_QUADRANT,
    BOTTOM_LEFT_QUADRANT
    }Screen_quadrant_enum;

enum {
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

// Memory Constants
static const Vector2 s_screen_origin = { HCENTER, VCENTER };

// Utility functions
float calc_distance(Vector2 point1, Vector2 point2);
float calc_enemy_approach_angle(Enemy_type enemy);

#endif