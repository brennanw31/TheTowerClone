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

// Coordinate helpers
#define HCENTER         ( SCREEN_WIDTH / 2 )
#define VCENTER         ( SCREEN_HEIGHT / 2 )

// Enumerations
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

// Memory Constants
static const Vector2 s_screen_origin = { HCENTER, VCENTER };

#endif