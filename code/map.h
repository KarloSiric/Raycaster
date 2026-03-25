#ifndef __MAP_H__
#define __MAP_H__

#include "shared.h"
#include <stdint.h>

#define WALL_NONE           0           // Numbering system for non-Wall.
#define WALL_COUNT          8           // Number of Wall textures used.

/**
 * Map Structure, used for holding the organization of the map
 */
typedef struct {
    
    uint8_t *grid;                      // dynamically allocted map grid
    int width;                          // map width
    int height;                         // map height
    float spawn_x;                      // map spawn_point x position.
    float spawn_y;                      // map spawn_point y position.
    float spawn_angle;                  // map spawn angle position
    int floor_index;                    // floor texture indexing.
    int ceiling_index;                  // ceiling texture indexing.
    char *name;                         // map name, dynamically alloc.
    int loaded;                         // map loaded or not.
    
} Map_t;

/**
 * @brief      Initializes the map.
 *
 * @param      map   The map
 */
void InitMap( Map_t *map );

/**
 * @brief      Loads a map.
 *
 * @param      map       The map
 * @param[in]  filepath  The filepath
 *
 * @return     Returns 0 on Success, ERROR otherwise.
 */
int LoadMap( Map_t *map, const char *filepath );

/**
 * @brief      Freeing the map resources allocated dynamically.
 *
 * @param      map   The map
 */
void FreeMap( Map_t *map );

/**
 * @brief      Gets the cell map.
 *
 * @param[in]  map   The map
 * @param[in]  x     The x position on the map.
 * @param[in]  y     The y position on the map.
 *
 * @return     The cell map.
 */
uint8_t GetCellMap( const Map_t *map, int x, int y );

/**
 * @brief      Determines if solid map.
 *
 * @param[in]  map   The map
 * @param[in]  x     The x position on the map.
 * @param[in]  y     The y position on the map.
 *
 * @return     Returns 0 on Success, ERROR otherwise.
 */
int IsSolidMap( const Map_t *map, int x, int y );

/**
 * @brief      Used for checking if it is withing the map bounds.
 *
 * @param[in]  map   The map
 * @param[in]  x     The x position on the map.
 * @param[in]  y     The y position on the map.
 *
 * @return     Returns 0 on Success, ERROR otherwise.
 */
int InBoundsMap( const Map_t *map, int x, int y );

#endif
