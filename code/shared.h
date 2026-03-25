#ifndef __SHARED_H__
#define __SHARED_H__ 

#define FPS                     60
#define FRAME_TIME              ( 1000 / FPS )

#define WINDOW_WIDTH            1280
#define WINDOW_HEIGHT           720

#define MAP_WIDTH               64
#define MAP_HEIGHT              64

#define TEXTURE_WIDTH           64
#define TEXTURE_HEIGHT          64

#define MINIMAP_SCALE           6
#define MINIMAP_X               20
#define MINIMAP_Y               20

#define RAY_COUNT               WINDOW_WIDTH

#define PI                      3.14159265359f
#define FOV_1                   ( PI / 3.0f )    
#define FOV_2                   ( 60 * PI / 180 )    

// MINIMAP COLORING
#define COLOR_MINIMAP_WALL      0xFFFFFF        // white    -> walls
#define COLOR_MINIMAP_FLOOR     0x000000        // black    -> ground
#define COLOR_MINIMAP_PLAYER    0x00FF00        // green    -> player
#define COLOR_MINIMAP_DIR       0xFF0000        // red      -> direction
#define COLOR_MINIMAP_BORDER    0x444444        // darkgray -> border

// FALLBACK TESTING COLORS FOR WALLS
#define COLOR_CEILING           0x1A1A1A        // dark ceiling
#define COLOR_FLOOR             0x333333        // gray floor
#define COLOR_WALL_DEFAULT      0x804000        // brown fallback wall

// WALL TYPES COLORINGS 1-9
#define COLOR_WALL_1            0x8B4513        // saddle brown (wood)
#define COLOR_WALL_2            0x696969        // dim gray (stone)
#define COLOR_WALL_3            0x2F4F4F        // dark slate (dungeon)
#define COLOR_WALL_4            0x8B0000        // dark red (brick)
#define COLOR_WALL_5            0x4A4A8B        // dark blue (marble)
#define COLOR_WALL_6            0x556B2F        // dark olive (mossy)
#define COLOR_WALL_7            0x4B0082        // indigo (mystic)
#define COLOR_WALL_8            0x2E2E2E        // dark gray (metal)
#define COLOR_WALL_9            0x5C4033        // dark brown (dirt)

#endif
