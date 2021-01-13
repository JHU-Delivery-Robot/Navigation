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
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//included math.h and stdio.h

/* ----------------------------------------------------------------------
 *   Defines, typedefs
 * ---------------------------------------------------------------------- */

/* Physical length of pixel */
#define PIXEL_PHYSICAL_LENGTH 2   /* Two centimeters? */

/* ----------------------------------------------------------------------
 *   Helper Functions/Typedefs
 * ---------------------------------------------------------------------- */

enum allocation_type {
    NO_ALLOCATION, SELF_ALLOCATED, STB_ALLOCATED
};

/* Structure for image input */
typedef struct {
	size_t size;
	int channels;
	uint8_t *pix;             /* bitmap data */
	int width;            /* width of bitmap array (map width in pixels) */
	int height;           /* height of bitmap array (map height in pixels) */
	enum allocation_type allocation_;
} Image;

/* ----------------------------------------------------------------------
 *   Function Implementations
 * ---------------------------------------------------------------------- */

//not sure if necessary, included from the potential map code
#define degreesToRadians(angleDegrees) (angleDegrees * PI / 180.0)
#define radiansToDegrees(angleRadians) (angleRadians * 180.0 / PI)

Obst_Map *load_obstacle_map(const char *pathname) {
	Image *im = malloc(sizeof(Image));

	if((im->pix = stbi_load(pathname, &im->width, &im->height, &im->channels, 0)) != NULL) {
		im->size = im->width * im->height * im->channels;
		im->allocation_ = STB_ALLOCATED;
	} else {
		free(im);
		return NULL;
	}

	Obst_Map *omap = malloc(sizeof(Obst_Map));

	unsigned map_size = im->width * im->height;

	omap->map = malloc(im->width * im->height * sizeof(Obst_Map_Px));

	if (im->channels > 1) {
		for(unsigned im_count = 0, map_count = 0; im_count < im->size && map_count < map_size; im_count += im->channels, map_count++) {
			*(omap->map + map_count) = (uint8_t)(*(im->pix + im_count) == 255 && *(im->pix + im_count + 1) == 255 && *(im->pix + im_count + 2) == 255 ? 1 : 0);
		}
	} else if (im->channels == 1) {
		for(unsigned im_count = 0, map_count = 0; im_count < im->size && map_count < map_size; im_count += im->channels, map_count++) {
			*(omap->map + map_count) = (uint8_t)(*(im->pix + im_count) == 255 ? 1 : 0);
		}
	} else {
		free_obst_map(omap);
		free(im->pix);
		free(im);
		return NULL;
	}

	free(im->pix);
	free(im);

	return omap;
}

void free_obst_map(Obst_Map *omap) {
	free(omap->map);
	free(omap);
}

int spatial_transform(int x_robot, int y_robot, int x, int y) {
	return 0;
}

//THIS IS NOT IN USE, JUST EXPERIMENTAL
int spatial_transform_2(Obst_Map* omap, int x_robot, int y_robot, int x, int y) {
	int x_1 = x + x_robot;
	int y_1 = y + y_robot;

	int x_2 = x_1;
	int y_2 = omap->height - 1 - y_1;

	return y_2 * omap->width + x_2;
}

//Everywhere you want to modify the value of x or y, use *x or *y instead (Shayan)
void walk_along_line_of_sight_more_horizontal(Obst_Map* omap, double angle, double slope, int x_robot, int y_robot, int* x, int* y) {
	int max = INT_MAX; //just to keep loop running until object is found
	for (int X = 0; X < max; X = (angle > PI / 2 && angle < 3 * PI / 2) ? X - 1 : X + 1) {
		int Y = floor(slope * ((double)X));
		if (omap->map[spatial_transform(x_robot, y_robot, X, Y)] == 1) {
			*x = X;
			*y = Y;
			break;
		}
		else if (omap->map[spatial_transform(x_robot, y_robot, X - 1, Y)] == 1) {
			*x = X - 1;
			*y = Y;
			break;
		}
	}
}

//Everywhere you want to modify the value of x or y, use *x or *y instead (Rishi)
void walk_along_line_of_sight_more_vertical(Obst_Map* omap, double angle, double slope, int x_robot, int y_robot, int* x, int* y) {
	int X = 0;
	for (int Y = 0; /*Upper Limit of Y is unknown for now*/; Y = (angle > 0 && angle < PI) ? Y + 1 : Y - 1) {
		X = floor(((double)Y) / slope);
		if (omap->map[spatial_transform(x_robot, y_robot, X, Y)] == 1) {
			*x = X;
			*y = Y;
		} else if (omap->map[spatial_transform(x_robot, y_robot, X, Y - 1)] == 1) {
			*x = X;
			*y = Y - 1;
		}
	}
}

//Everywhere you want to modify the value of x or y, use *x or *y instead (Jared)
void walk_along_line_of_sight_non_peculiar(Obst_Map* omap, double angle, int x_robot, int y_robot, int* x, int* y) {
	for (int X = 0, Y = 0; /*limits?*/;) {
		//iterating x values
		if ((angle > 0.0 && angle < PI / 2.0) || (angle > 3.0 * PI / 2.0 && angle < 2.0 * PI))  //quadrants I,IV
		{
			X++;
		} else if (angle > PI / 2.0 && angle < 3.0 * PI / 2.0) //quadrants II,III
		{
			X--;
		}

		//iterating y values 
		if (angle > 0.0 && angle < PI) //quadrants I,II
		{
			Y++;
		} else if (angle > PI && angle < 2.0 * PI) //III,IV
		{
			Y--;
		}
	}
}

//look into bubble rebound algorithm? quad tree?
//figure out how to get info from lidar, ask Ben, using a scanner for now
//need second set of points? included here; not sure about angle
//returns -1.0d0
double dist_to_obstacle(Obst_Map* omap, Pixel_Dimen x, Pixel_Dimen y, double angle) {
	int X = 0, Y = 0;

	if (ceilf(angle) == angle && (int)(angle) % 45 == 0) {
		walk_along_line_of_sight_non_peculiar(omap, angle, x, y, &X, &Y);
	} else {
		double slope = tan(angle);

		if (fabs(slope) < 1.0) {
			walk_along_line_of_sight_more_horizontal(omap, angle, slope, x, y, &X, &Y);
		} else {
			walk_along_line_of_sight_more_vertical(omap, angle, slope, x, y, &X, &Y);
		}
	}

	//Access x and y normally. These are the coordinates now.

	return -1.0;
}

double dist_to_obstacle_limited(Obst_Map* omap, Pixel_Dimen x, Pixel_Dimen y, double angle, Pixel_Dimen limit) {
	double distance = dist_to_obstacle(omap, x, y, angle);
	if (distance <= limit) {
		return distance;
	}
	return -1.0;
}

Physical_Dimen pixel_to_phys_dimen(Pixel_Dimen length) {
	return length * PIXEL_PHYSICAL_LENGTH;
}

Pixel_Dimen phys_to_pixel_dimen(Physical_Dimen length) {
	return length / PIXEL_PHYSICAL_LENGTH;
}
