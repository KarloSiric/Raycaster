/*======================================================================
	 File: raycast.c
	 Project: MyProjects
	 Author: karlosiric <email@example.com>
	 Created: 2026-03-18 19:31:10
	 Last Modified by: karlosiric
	 Last Modified: 2026-03-19 10:14:23
	 ---------------------------------------------------------------------
	 Description:

	 ---------------------------------------------------------------------
	 License:
	 Company:
	 Version: 0.1.0
 ======================================================================
																	   */

#include <SDL2/SDL.h>
#include <math.h>

#define SCREEN_WIDTH            1920
#define SCREEN_HEIGHT           1080
#define PLAYER_POV              90
#define CELL_SIZE               25

#define PLAYER_COLOR            0x00FF00
#define PLAYER_SPEED            .25f
#define PLAYER_ROT_SPEED        .10f

#define RAYCAST_STEP            .01f
#define RAYCAST_COLOR           0xFFFF00

#define CEILING_COLOR           0x333333
#define FLOOR_COLOR             0x666666

#define FOV                     ( M_PI / 3 )

#define MAP_WIDTH               24
#define MAP_HEIGHT              24

#define TEXTURE_WIDTH           64
#define TEXTURE_HEIGHT          64
#define NUM_OF_WALL_TEXTURES    8

int worldMap[MAP_HEIGHT][MAP_WIDTH] = {
    
	{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 7, 7, 7, 7, 7, 7, 7, 7 },
	{ 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 7 },
	{ 4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7 },
	{ 4, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7 },
	{ 4, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 7 },
	{ 4, 0, 4, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 7, 7, 0, 7, 7, 7, 7, 7 },
	{ 4, 0, 5, 0, 0, 0, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 7, 0, 0, 0, 7, 7, 7, 1 },
	{ 4, 0, 6, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 5, 7, 0, 0, 0, 0, 0, 0, 8 },
	{ 4, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 7, 1 },
	{ 4, 0, 8, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 5, 7, 0, 0, 0, 0, 0, 0, 8 },
	{ 4, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 5, 7, 0, 0, 0, 7, 7, 7, 1 },
	{ 4, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 5, 5, 5, 5, 7, 7, 7, 7, 7, 7, 7, 1 },
	{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
	{ 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4 },
	{ 6, 6, 6, 6, 6, 6, 0, 6, 6, 6, 6, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
	{ 4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 6, 0, 6, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3 },
	{ 4, 0, 0, 0, 0, 0, 0, 0, 0, 4, 6, 0, 6, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2 },
	{ 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 2, 0, 0, 5, 0, 0, 2, 0, 0, 0, 2 },
	{ 4, 0, 0, 0, 0, 0, 0, 0, 0, 4, 6, 0, 6, 2, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2 },
	{ 4, 0, 6, 0, 6, 0, 0, 0, 0, 4, 6, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 2 },
	{ 4, 0, 0, 5, 0, 0, 0, 0, 0, 4, 6, 0, 6, 2, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2 },
	{ 4, 0, 6, 0, 6, 0, 0, 0, 0, 4, 6, 0, 6, 2, 0, 0, 5, 0, 0, 2, 0, 0, 0, 2 },
	{ 4, 0, 0, 0, 0, 0, 0, 0, 0, 4, 6, 0, 6, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2 },
	{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3 }
    
};

static const char *wall_texture_paths[NUM_OF_WALL_TEXTURES] = {
    
    "assets/textures/[SUB]wall_A_003.bmp",
    "assets/textures/[SUB]wall_A_003.bmp",
    "assets/textures/[SUB]wall_A_003.bmp",
    "assets/textures/[SUB]wall_A_003.bmp",
    "assets/textures/[SUB]wall_A_003.bmp",
    "assets/textures/[SUB]wall_A_003.bmp",
    "assets/textures/[SUB]wall_A_003.bmp",
    "assets/textures/[SUB]wall_A_003.bmp"
    // "assets/textures/[SUB]Modern_Wall_001.bmp",
    // "assets/textures/[SUB]Modern_Wall_001.bmp",
    // "assets/textures/[SUB]Modern_Wall_001.bmp",
    // "assets/textures/[SUB]Modern_Wall_001.bmp",
    // "assets/textures/[SUB]Modern_Wall_001.bmp",
    // "assets/textures/[SUB]Modern_Wall_001.bmp",
    // "assets/textures/[SUB]Modern_Wall_001.bmp",
    
};

static const char *floor_texture_path = "assets/textures/[SUB]floor_A_005.bmp";
static const char *ceiling_texture_path = "assets/textures/[SUB]ceiling_A_001.bmp";

static Uint32 mapColors[9] = { 
    
    0x000000, // empty
    0x4A3728, // dark brown stone
    0x3A3A3A, // dark gray
    0x2A2A4A, // dark blue stone
    0x4A4A2A, // mossy stone
    0x5A3A5A, // purple stone
    0x2A4A4A, // dark teal
    0x5A5A5A, // light gray
    0x3A3A3A, // medium gray
    
};

SDL_Surface *wall_textures[NUM_OF_WALL_TEXTURES];
SDL_Surface *floor_texture;
SDL_Surface *ceiling_texture;

typedef struct {
    
	float x, y, angle;
	float speed;
	float rot_speed;
    
} Player;

typedef struct {
    
    float pos_x, pos_y;                             // start position of the ray, from player positions
    float dir_x, dir_y;                             // directional vector of the ray itself
    float hit_x, hit_y;                             // position where it hit the wall so x and y
    float distance;                                 // distance to the wall
    int wall_type;                                  // which wall number type it is 1-8
    int side;                                       // either vertical 0 or horizontal wall 1, so either side or front ( west/east or north/south )
    
} Ray;

static Player player = { 1.5f, 1.5f, 0.0f, PLAYER_SPEED, PLAYER_ROT_SPEED };

void DrawPlayer( SDL_Surface *surface, Player *player ) {
	SDL_Rect player_rect = {
		player->x * CELL_SIZE,
		player->y * CELL_SIZE,
		4,
		4
	};
    
	SDL_FillRect( surface, &player_rect, PLAYER_COLOR );
	return;
}

void DrawMapGrid( SDL_Surface *surface ) {
	SDL_Rect wallRect;
	Uint32 wallColor;
	for ( int i = 0; i < MAP_HEIGHT; i++ ) {
		for ( int j = 0; j < MAP_WIDTH; j++ ) {
			// if there is a number we need to draw that then
			if ( worldMap[i][j] ) {
				wallRect.x = j * CELL_SIZE;
				wallRect.y = i * CELL_SIZE;
				wallRect.w = CELL_SIZE;
				wallRect.h = CELL_SIZE;
				wallColor = mapColors[worldMap[i][j]];
				SDL_FillRect( surface, &wallRect, wallColor );
			}
		}
	}
    
	return;
}

int ValidPosition( float x, float y ) {
    // invalid if out of map bounds
    if ( x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT ) {
        return 0;
    }
    
    if ( worldMap[( int )y][( int )x] != 0 ) {
        return 0;
    }
    
    return 1;
}

void MovePlayer( Player *player ) {
    const Uint8 *keys = SDL_GetKeyboardState( NULL ); 
    // Moving forward walking
    if ( keys[SDL_SCANCODE_W] ) {
        float new_x = player->x + cos( player->angle ) * player->speed;
        float new_y = player->y + sin( player->angle ) * player->speed;
        if ( ValidPosition( new_x, player->y ) ) {
            player->x = new_x;
        }
        if ( ValidPosition( player->x, new_y ) ) {
            player->y = new_y;
        }
    }
    // Moving backwards walking
    if ( keys[SDL_SCANCODE_S] ) {        
        float new_x = player->x - cos( player->angle ) * player->speed;
        float new_y = player->y - sin( player->angle ) * player->speed;
        if ( ValidPosition( new_x, player->y ) ) {
            player->x = new_x;
        }
        if ( ValidPosition( player->x, new_y ) ) {
            player->y = new_y;
        }
    }
    // Moving side to side - strafing
    if ( keys[SDL_SCANCODE_A] ) {
        player->angle -= PLAYER_ROT_SPEED;
    }
    if ( keys[SDL_SCANCODE_D] ) {
        player->angle += PLAYER_ROT_SPEED;
    }
    
    return ;   
}

void DrawLine( SDL_Surface *surface, float x0, float y0, float x1, float y1, Uint32 color ) {
    float dx = x1 - x0;
    float dy = y1 - y0;
    float length = sqrt( dx*dx + dy*dy );
    float step = 0.1f;  
    int steps = ( int )( length / step );       
    for ( int i = 0; i <= steps; i++ ) {
        float t = ( ( float )i / ( float )steps );
        int px = ( int )( ( x0 + dx * t ) * CELL_SIZE );
        int py = ( int )( ( y0 + dy * t ) * CELL_SIZE );
        SDL_Rect pixel = { px, py, 1, 1 };
        SDL_FillRect( surface, &pixel, color );
    }
    
    return ;
}

void LoadTextures( SDL_Surface *targetSurface ) {
    // @NOTE: All paths for the wall textures
    for ( int i = 0; i < NUM_OF_WALL_TEXTURES; i++ ) {
        SDL_Surface *temp = SDL_LoadBMP( wall_texture_paths[i] );
        if ( !temp ) {
            fprintf( stderr, "SDL_LoadBMP: Error: Failed to load texture %d: %s\n", i, SDL_GetError() );
            continue;
        }
        wall_textures[i] = SDL_ConvertSurface(temp, targetSurface->format, 0 );
        SDL_FreeSurface( temp );
        if ( !wall_textures[i] ) {
            fprintf( stderr, "SDL_ConvertSurface: Error: Failed to convert texture: %d: %s\n", i, SDL_GetError() ); 
        } 
    }
    
    // @NOTE: Floor and ceiling adding
    SDL_Surface *temp = SDL_LoadBMP( floor_texture_path );
    if ( !temp ) {
        fprintf( stderr, "SDL_LoadBMP: Error: Failed to load texture %s: %s\n", floor_texture_path, SDL_GetError() );
    }
    floor_texture = SDL_ConvertSurface( temp, targetSurface->format, 0 );
    SDL_FreeSurface( temp );
    if ( !floor_texture ) {
        fprintf( stderr, "SDL_ConvertSurface: Error: Failed to convert texture: %s: %s\n", floor_texture_path, SDL_GetError() ); 
    } 
    
    temp = SDL_LoadBMP( ceiling_texture_path );
    if ( !temp ) {
        fprintf( stderr, "SDL_LoadBMP: Error: Failed to load texture %s: %s\n", ceiling_texture_path, SDL_GetError() );
    }  
    
    ceiling_texture = SDL_ConvertSurface( temp, targetSurface->format, 0 );
    SDL_FreeSurface( temp );    
    if ( !ceiling_texture ) {
        fprintf( stderr, "SDL_ConvertSurface: Error: Failed to convert texture: %s: %s\n", ceiling_texture_path, SDL_GetError() ); 
    }
    
    return ;
}

void DrawWall( SDL_Surface *surface, Ray *ray, int column ) {
    int wall_height = ( int )( SCREEN_HEIGHT / ray->distance * 1.5f );
    int draw_start_unclamped = ( SCREEN_HEIGHT - wall_height ) / 2;
    int draw_start = draw_start_unclamped;
    int draw_end = draw_start + wall_height;
    
    if ( draw_start < 0 ) {
        draw_start = 0;
    }
    if ( draw_end >= SCREEN_HEIGHT ) {
        draw_end = SCREEN_HEIGHT - 1;
    }

    // Direct pixel buffer access - much faster than SDL_FillRect per pixel
    Uint32 *pixels = ( Uint32 * )surface->pixels;
    int surface_pitch = surface->pitch / 4;

    int texture_id = ray->wall_type - 1;
    if ( texture_id < 0 || texture_id >= NUM_OF_WALL_TEXTURES || !wall_textures[texture_id] ) {
        // @NOTE: we fallback to the regular coloring if we dont have the texture
        Uint32 color = mapColors[ray->wall_type];
        for ( int y = draw_start; y < draw_end; y++ ) {
            pixels[y * surface_pitch + column] = color;
        }
        return;
    }

    SDL_Surface *texture = wall_textures[texture_id];
    Uint32 *tex_pixels = ( Uint32 * )texture->pixels;
    int tex_pitch = texture->pitch / 4;

    // @NOTE(Karlo): detecting side wall first then frontal ( side = 0, front = 1 )
    float wall_x;
    if ( ray->side == 0 ) {
        wall_x = ray->hit_y - floor( ray->hit_y );
    } else {
        wall_x = ray->hit_x - floor( ray->hit_x );
    }

    int tex_x = ( int )( wall_x * texture->w );
    if ( tex_x >= texture->w ) {
        tex_x = texture->w - 1;
    }

    // Pre-calculate fog once per column (same for whole wall strip)
    float fog = 1.0f - ( ray->distance / 12.0f );
    if ( fog < 0.15f ) fog = 0.15f;
    if ( fog > 1.0f ) fog = 1.0f;

    for ( int y = draw_start; y < draw_end; y++ ) {
        // Calculate texture y coordinate
        float tex_pos = ( float )( y - draw_start_unclamped ) / wall_height;
        int tex_y = ( int )( tex_pos * texture->h );
        if ( tex_y >= texture->h ) {
            tex_y = texture->h - 1;
        }

        // Get pixel from texture
        Uint32 color = tex_pixels[tex_y * tex_pitch + tex_x];

        // Apply fog
        int r = ( ( color >> 16 ) & 0xFF ) * fog;
        int g = ( ( color >> 8 ) & 0xFF ) * fog;
        int b = ( color & 0xFF ) * fog;
        color = ( r << 16 ) | ( g << 8 ) | b;

        // Direct pixel write
        pixels[y * surface_pitch + column] = color;
    }

    return;
}

void DrawFloorCeiling( SDL_Surface *surface, Player *player ) {
    Uint32 *pixels = ( Uint32 * )surface->pixels;
    int pitch = surface->pitch / 4;
    
    Uint32 *floor_pixels = ( Uint32 * )floor_texture->pixels;
    int floor_pitch = floor_texture->pitch / 4;
    
    Uint32 *ceiling_pixels = ( Uint32 *)ceiling_texture->pixels;
    int ceiling_pitch = ceiling_texture->pitch / 4;
    
    for ( int y = 0; y < SCREEN_HEIGHT; y++ ) {      
        int is_floor = y > SCREEN_HEIGHT / 2;
        int p = is_floor ? ( y - ( SCREEN_HEIGHT / 2 ) ) : ( ( SCREEN_HEIGHT / 2 ) - y );
        float row_distance = ( ( (float)SCREEN_HEIGHT / (float)2 ) ) / ( float )p;
        for ( int x = 0; x < SCREEN_WIDTH; x++ ) {
            // @NOTE: We process each pixel here now, so each x for that y column basically on the floor so we process all pixels in that row
            float ray_angle = player->angle - ( FOV / 2 ) + ( FOV * x / SCREEN_WIDTH );
            float floor_x = player->x + cos( ray_angle ) * row_distance;
            float floor_y = player->y + sin( ray_angle ) * row_distance;
            int tex_x = ( int )( floor_x * floor_texture->w ) % floor_texture->w; 
            int tex_y = ( int )( floor_y * floor_texture->h ) % floor_texture->h;
            if ( tex_x < 0 ) {
                tex_x += floor_texture->w;
            }
            if ( tex_y < 0 ) {
                tex_y += floor_texture->h;
            }
            Uint32 color;
            
            float fog = 1.0f - ( row_distance / 12.0f );
            if ( fog < 0.15f ) {
                fog = 0.15f;
            }
            if ( fog > 1.0f ) {
                fog = 1.0f;
            }
            
            if ( is_floor ) {
                color = floor_pixels[ tex_y * floor_pitch + tex_x ];
            } else {
                color = ceiling_pixels[ tex_y * ceiling_pitch + tex_x ];
            }
            
            int r = ( ( color >> 16 ) & 0xFF ) * fog;
            int g = ( ( color >> 8 ) & 0xFF ) * fog;
            int b = ( color & 0xFF ) * fog;
            
            color = ( r << 16 ) | ( g << 8 ) | b;
            pixels[ y * pitch + x ] = color;
        } 
    }
    
    return ;
}

void DrawSingleRay( Ray *ray ,SDL_Surface *surface, Player *player, float angle ) {
    // every frame this will be drawn and initialized
    ray->pos_x = player->x;
    ray->pos_y = player->y;
    ray->dir_x = cos( angle );
    ray->dir_y = sin( angle );
    // looping until we hit the wall or not depends
    while( 1 ) {
        ray->pos_x += ray->dir_x * RAYCAST_STEP;
        ray->pos_y += ray->dir_y * RAYCAST_STEP;        
          
        int ray_map_x = ( int )ray->pos_x;
        int ray_map_y = ( int )ray->pos_y;
        
        // ray is out of the map
        if ( ray_map_x < 0 || ray_map_x >= MAP_WIDTH || ray_map_y < 0 || ray_map_y >= MAP_HEIGHT ) {
            ray->distance = 9999;
            break;
        } 
        if ( worldMap[ray_map_y][ray_map_x] > 0 ) {
            ray->wall_type = worldMap[ray_map_y][ray_map_x];
            ray->hit_x = ray->pos_x;
            ray->hit_y = ray->pos_y;
            float dx = ray->hit_x - player->x;
            float dy = ray->hit_y - player->y;
            ray->distance = sqrt( dx * dx + dy * dy );
            break;
        }
    }
    
    DrawLine( surface, player->x, player->y, ray->pos_x, ray->pos_y, RAYCAST_COLOR );
    
    return ;
}

void CastRay( Ray *ray ,SDL_Surface *surface, Player *player, float angle ) {
    // every frame this will be drawn and initialized
    ray->pos_x = player->x;
    ray->pos_y = player->y;
    ray->dir_x = cos( angle );
    ray->dir_y = sin( angle );
    
    // looping until we hit the wall or not depends
    while( 1 ) {
        ray->pos_x += ray->dir_x * RAYCAST_STEP;
        ray->pos_y += ray->dir_y * RAYCAST_STEP;        
          
        int ray_map_x = ( int )ray->pos_x;
        int ray_map_y = ( int )ray->pos_y;
        
        // ray is out of the map
        if ( ray_map_x < 0 || ray_map_x >= MAP_WIDTH || ray_map_y < 0 || ray_map_y >= MAP_HEIGHT ) {
            ray->distance = 9999;
            break;
        } 
        if ( worldMap[ray_map_y][ray_map_x] > 0 ) {
            ray->wall_type = worldMap[ray_map_y][ray_map_x];
            ray->hit_x = ray->pos_x;
            ray->hit_y = ray->pos_y;
            float dx = ray->hit_x - player->x;
            float dy = ray->hit_y - player->y;
            
            // Adding proper fisheye correction for curving walls
            ray->distance = ( 
                            ( ( ray->hit_x - player->x ) * cos( player->angle ) ) + 
                            ( ( ray->hit_y - player->y ) * sin( player->angle ) ) 
                              );
            // Determine which wall face was hit by checking nearest edge
            float cell_x = ray->hit_x - floor( ray->hit_x );
            float cell_y = ray->hit_y - floor( ray->hit_y );
            float dist_x = fmin( cell_x, 1.0f - cell_x );
            float dist_y = fmin( cell_y, 1.0f - cell_y );

            if ( dist_x < dist_y ) {
                ray->side = 0;  // Hit vertical wall (left/right edge)
            } else {
                ray->side = 1;  // Hit horizontal wall (top/bottom edge)
            }
            break;
        }
    } 
    
    return ;
}

void RenderScene( SDL_Surface *surface, Player *player ) {
    Ray ray;
    for ( int x = 0; x < SCREEN_WIDTH; x++ ) {
        // get the angle for this ray
        float angle = player->angle - ( FOV/2 ) + ( FOV * x / SCREEN_WIDTH );
        CastRay( &ray, surface, player, angle );
        // now we draw a wall for those rays and information
        DrawWall( surface, &ray, x );
    }
    
    return ;
}

int main( int /*argc*/, const char * /*argv*/[] ) {
	SDL_Surface *winSurface = NULL;
	SDL_Window *window = NULL;
	SDL_Event event;
	int running = 1;
    
	// Now we initialize the SDL properly, and it will return -1 on error
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		fprintf( stderr, "SDL_Init: Error initializing SDL: %s\n", SDL_GetError() );
		SDL_Delay( 3000 );
		exit( EXIT_FAILURE );
	}
    // @NOTE: We load the textures that we will need
    
	window = SDL_CreateWindow( "RAYCASTER", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
	// @NOTE(Karlo): make sure that the window is being created
	if ( !window ) {
		fprintf( stderr, "SDL_CreateWindow: Error creating window: %s\n", SDL_GetError() );
		SDL_Delay( 3000 );
		exit( EXIT_FAILURE );
	}
	winSurface = SDL_GetWindowSurface( window );
	if ( !winSurface ) {
		fprintf( stderr, "SDL_GetWindowSurface: Error getting window surface: %s\n", SDL_GetError() );
		SDL_Delay( 3000 );
		exit( EXIT_FAILURE );
	}
    LoadTextures( winSurface );
	SDL_FillRect( winSurface, NULL, SDL_MapRGB( winSurface->format, 0, 0, 0 ) );
	SDL_UpdateWindowSurface( window );
	while ( running ) {
		while ( SDL_PollEvent( &event ) ) {
			if ( event.type == SDL_QUIT ) {
				running = 0;
			}
		}
        SDL_FillRect( winSurface, NULL, 0x000000 );
        
        MovePlayer( &player );
        DrawFloorCeiling( winSurface, &player );
        RenderScene( winSurface, &player );       
        
		SDL_UpdateWindowSurface( window );
		SDL_Delay( 16 );
	}
	SDL_DestroyWindow( window );
	SDL_Quit();

	return 0;
}
