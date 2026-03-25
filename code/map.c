/*======================================================================
   File: map.c
   Project: MyProjects
   Author: karlosiric <email@example.com>
   Created: 2026-03-24 20:15:10
   Last Modified by: ksiric
   Last Modified: 2026-03-25 09:49:52
   ---------------------------------------------------------------------
   Description:
       
   ---------------------------------------------------------------------
   License: 
   Company: 
   Version: 0.1.0
 ======================================================================
                                                                       */

#include "map.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


global int ReadNextLine( FILE *file, char *buffer, int bufsize ) {
    while( fgets( buffer, bufsize, file ) ) {
        // skip the empty lines
        if ( buffer[0] == '\n' || buffer[0] == '\0' ) {
            continue;
        }
        // skip the comments
        if ( buffer[0] == '#' ) {
            continue;
        } 
        // got the real line
        return 1;
    }
    
    // either EOF or some error occured
    return 0;
}


void InitMap( Map_t *map ) {  
    memset( map, 0, sizeof( Map_t ) );  
    
    return ;
} 

/* @NOTE Map loading relies on parsing the custom made .map file format */
int LoadMap( Map_t *map, const char *filepath ) {
    if ( !map ) {
        fprintf( stderr, "LoadMap: Error: map pointer is NULL" );
        return ( -1 );
    }   
    if ( !filepath ) {
        fprintf( stderr, "LoadMap: Error: filepath pointer is NULL" );
        return ( -1 );
    }   
    
    InitMap( map );
    
    FILE *file = fopen( filepath, "r" );
    if ( !file ) {
        fprintf( stderr, "LoadMap: Error: Cannot open file: %s\n", filepath );
        return ( -1 );
    }  
    
    char line[256];
    while ( fgets( line, sizeof( line ), file ) ) {
        // @NOTE Reading NAME
        if ( ReadNextLine( file, line, sizeof( line ) ) == 0 ) {
            fprintf( stderr, "LoadMap: ReadNextLine: Error: Missing NAME\n" );
            fclose( file );
            return ( -1 );
        }  
        if ( strncmp( line, "NAME ", 5 ) != 0 ) {
            fprintf( stderr, "LoadMap: Error: Expected NAME, got: %s\n", line );
            fclose( file );
            return ( -1 );
        }
        char *map_name = line + 5;
        size_t mapname_len = strlen( map_name );
        if ( mapname_len > 0 && map_name[mapname_len - 1] == '\n' ) {
            map_name[mapname_len - 1] = '\0';
            mapname_len--;  
        }
            
        map->name = ( char *)malloc( mapname_len * sizeof( char ) );
        if ( !map->name ) {
            fprintf( stderr, "LoadMap: Error: Failed to allocate memory for name!\n" );
            fclose( file );
            return ( -1 );
        }
        strcpy( map->name, map_name );            
        
        // @NOTE Reading WIDTH HEIGHT
        if ( ReadNextLine( file, line, sizeof( line ) ) == 0 ) {
            fprintf( stderr, "ReadNextLine: Error: Missing WIDTH\n" );
            fclose( file );
            return ( -1 );
        }
        
        
               
        
        
    }
    
    
    
       
    
}
