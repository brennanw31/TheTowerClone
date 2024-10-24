#define SCREEN_WIDTH    ( 1280 )
#define SCREEN_HEIGHT   ( 720 )
#define FRAMERATE       ( 240 )

#define HCENTER         ( SCREEN_WIDTH / 2 )
#define VCENTER         ( SCREEN_HEIGHT / 2 )
#define CIRCLE_RADIUS   ( 250.0f )
#define SPAWN_BUFFER    ( 150.0f )
#define MAX_ENEMIES     ( 100 )

#define BASIC_ENEMY_HEIGHT  ( 20.0f )
#define BASIC_ENEMY_WIDTH   ( 20.0f )

typedef enum{
    UPPER_QUARTILE,  
    RIGHT_QUARTILE,  
    LOWER_QUARTILE,  
    LEFT_QUARTILE  
    }Quartile_type;

enum{
    TOP_LINE,
    RIGHT_LINE,
    BOTTOM_LINE,
    LEFT_LINE,
    ENEMY_BBOX_LINE_COUNT
    };

// Game mechanic functions
bool detect_collisions(Vector2 sweeper_posn, Rectangle enemy);
void move_enemies(Rectangle *enemies, int enemy_count, int speed);
void spawn_basic_enemy(Rectangle *enemy);
void get_basic_enemy_lines(Rectangle enemy, Vector2 *out_start_points, Vector2 *out_end_points);
void draw_enemies( Rectangle *enemies, int enemy_count);
void draw_tower(Vector2 *posn, float speed);

// Utility functions
void rect_array_remove( Rectangle *arr, int *arr_size, int index );