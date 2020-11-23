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

#include <stddef.h>
#include <stdint.h>
#include <math.h> 
#include <stdlib.h>
#include "obstacle_map.h"

//From stb_image library at https://github.com/nothings/stb
#include "stb_image.h"
//included math.h and stdio.h 

/*
 * Defines, typedefs
 */

/* Physical length of pixel */
#define PIXEL_PHYSICAL_LENGTH 2   /* Two centimeters? */

/* 
 * Helper Functions
 */

/*
 * Function Implementations
 */

//not sure if necessary, included from the potential map code
#define degreesToRadians(angleDegrees) (angleDegrees * PI / 180.0)     
#define radiansToDegrees(angleRadians) (angleRadians * 180.0 / PI)

Obst_Map *load_obstacle_map(const char *pathname) {
	Obst_Map *omap = malloc(sizeof(Obst_Map));

	if((omap->map = stbi_load(pathname, &omap->width, &omap->height, &omap->channels, 0)) != NULL) {
		omap->size = omap->width * omap->height * omap->channels;
		omap->allocation_ = STB_ALLOCATED;
	}

	return omap;
}

void free_obst_map(Obst_Map *omap) {
	free(omap->map);
	free(omap);
}

double dist_to_obstacle(Pixel_Dimen x1, Pixel_Dimen y1, double angle) {
	//look into bubble rebound algorithm? quad tree?
	//figure out how to get info from lidar, ask Ben, using a scanner for now
	//need second set of points? included here; not sure about angle
	//returns -1.0d0
	
	return -1.0;
}

double dist_to_obstacle_limited(Pixel_Dimen x, Pixel_Dimen y, double angle, Pixel_Dimen limit) {
	return -1.0;
}

Physical_Dimen pixel_to_phys_dimen(Pixel_Dimen length) {
	return length * PIXEL_PHYSICAL_LENGTH;
}

Pixel_Dimen phys_to_pixel_dimen(Physical_Dimen length) {
	return length / PIXEL_PHYSICAL_LENGTH;
}