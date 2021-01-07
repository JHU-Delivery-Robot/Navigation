/* 
 * JHU Deliverbot Navigation Group 
 * Navigation simulation system 
 * Obstacle map management --- header file 
 *
 * @avnoks (GitHub)  23 Sep 2020
 *
 */

/* A NOTICE (23 Sep 2020): We're at an early stage.  Feel free to change 
 * things like function names and types to things that make more sense.
 */

#ifndef OBSTACLE_MAP_H
#define OBSTACLE_MAP_H

#include <stddef.h>
#include <stdint.h>
#include "stb_image.h"

/* ----------------------------------------------------------------------
 *   Defines, typedefs
 * ---------------------------------------------------------------------- */

#define PI 3.14159265358979323

typedef uint8_t Obst_Map_Px;     /* Single pixel in obstacle map */
typedef unsigned Pixel_Dimen;
/* A length in our physical world with real units (could be meters, 
 * cm, etc --- we need to decide).  Do we want to use a double for this? */
typedef unsigned Physical_Dimen;

/* Structure for storing obstacle map and relevant information */
typedef struct {
	Obst_Map_Px *map;     /* bitmap data */
	Pixel_Dimen width;            /* width of bitmap array (map width in pixels) */
	Physical_Dimen height;           /* height of bitmap array (map height in pixels) */
} Obst_Map;


/* ----------------------------------------------------------------------
 *   Function prototypes
 * ---------------------------------------------------------------------- */

/*
 * Allocates an Obst_Map and fills it with data from bitmap stored at pathname
 * Args: 
 *   pathname  -  Pathname of bitmap file
 * Returns:
 *   ptr to freshly allocated Obst_Map containing data in loaded bitmap 
 *      (you must free this pointer when you are done with the map)
 */
Obst_Map *load_obstacle_map(const char *pathname);

/*
 * Frees a heap-allocated Obst_Map struct
 * Args: 
 *   omap - pointer heap-allocated to Obst_Map struct
 * Returns:
 *   N/A
 */
void free_obst_map(Obst_Map *omap);

/*
 * Computes distance to nearest obstacle from a given point in a given 
 * direction.
 * Args: 
 *   x      - x coordinate of point to start searching from
 *   y      - y coordinate of point to start searching from
 *   angle  - direction to search for obstacles in
 * Returns: 
 *   double float (for some semblance of precision) of distance 
 *     in pixel units from given point to nearest obstacle
 */
double dist_to_obstacle(Pixel_Dimen x, Pixel_Dimen y, double angle);

/*
 * Computes distance to nearest obstacle within a specified distance 
 * from a given point in a given direction.
 * Args: 
 *   x      - x coordinate of point to start searching from
 *   y      - y coordinate of point to start searching from
 *   angle  - direction to search for obstacles in
 *   limit  - distance, in pixel units, to stop searching at
 * Returns: 
 *   double float (for some semblance of precision) of distance 
 *     in pixel units from given point to nearest obstacle IF
 *     obstacle was found within limit.  OTHERWISE, returns -1.0d0 
 *     to indicate that no obstacle was found within limit.
 */
double dist_to_obstacle_limited(Pixel_Dimen x, Pixel_Dimen y, double angle, Pixel_Dimen limit);


/* Functions to convert between pixel lengths and corresponding physical 
 * lengths and vice versa.  Should we inline these? */
Physical_Dimen pixel_to_phys_dimen(Pixel_Dimen length);
Pixel_Dimen phys_to_pixel_dimen(Physical_Dimen length);

#endif