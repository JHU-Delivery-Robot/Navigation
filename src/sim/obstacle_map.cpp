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

#include <cstddef>
#include <cstdint>
#include <cmath>
#include <cstdlib>
#include <string>
#include <vector>
#include "obstacle_map.hpp"


#include "stb/stb_image.h"
//included math.h and stdio.h 

/* ----------------------------------------------------------------------
 *   Defines
 * ---------------------------------------------------------------------- */

/* Physical length of pixel */
#define PIXEL_PHYSICAL_LENGTH 2   /* Two centimeters? */
#define ERR_CODE_COLLISION -2.0   /* Sensor is inside an object */

/* ----------------------------------------------------------------------
 *   Typedefs
 * ---------------------------------------------------------------------- */

using std::string;
using std::vector;

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

ObstMap::ObstMap(string pathname) {
	Image *im = (Image *)malloc(sizeof(Image));

	if((im->pix = stbi_load(pathname.c_str(), &im->width, &im->height, &im->channels, 0)) != NULL) {
		im->size = im->width * im->height * im->channels;
		im->allocation_ = STB_ALLOCATED;
	} else {
		free(im);
		return;
	}

	height = im->height;
	width = im->width;

	unsigned map_size = width * height;

	map.resize(map_size);

	if (im->channels > 1) {
		for(unsigned im_count = 0, map_count = 0; im_count < im->size && map_count < map_size; im_count += im->channels, map_count++) {
			map[map_count] = (Obst_Map_Px)(*(im->pix + im_count) == 255 && *(im->pix + im_count + 1) == 255 && *(im->pix + im_count + 2) == 255 ? 1 : 0);
		}
	} else if (im->channels == 1) {
		for(unsigned im_count = 0, map_count = 0; im_count < im->size && map_count < map_size; im_count += im->channels, map_count++) {
			map[map_count] = (Obst_Map_Px)(*(im->pix + im_count) == 255 ? 1 : 0);
		}
	}

	free(im->pix);
	free(im);
}

Pixel_Dimen ObstMap::CoordsToBitmapIndex(Pixel_Dimen x, Pixel_Dimen y) {
	Pixel_Dimen x_2 = x;
	Pixel_Dimen y_2 = y;

	return y_2 * width + x_2;
}

double ObstMap::distance(Pixel_Dimen x0, Pixel_Dimen y0, Pixel_Dimen x1, Pixel_Dimen y1) {
	Pixel_Dimen dx = 0, dy = 0;

	if (x1 > x0) {
		dx = x1 - x0;
	} else {
		dx = x0 - x1;
	}

	if (y1 > y0) {
		dy = y1 - y0;
	} else {
		dy = y0 - y1;
	}

	return sqrt(dx * dx + dy * dy);
}

double ObstMap::distToObstacle(Pixel_Dimen x0, Pixel_Dimen y0, double angle) {
	if (map.at(CoordsToBitmapIndex(x0, y0)) == 1) {
		return ERR_CODE_COLLISION;
	}

	Pixel_Dimen x = x0, y = y0;

	double dx = fabs(cos(angle));
	int sx = (angle >= 0 && angle < PI / 2) || (angle > 3 * PI / 2 && angle <= 2 * PI) ? 1 : -1;
	double dy = fabs(sin(angle));
	int sy = angle > 0 && angle < PI ? 1 : -1;
	double err = (dx > dy ? dx : -dy) / 2.0, e2;

	while (x < width && y < height) {
		if (map.at(CoordsToBitmapIndex(x, y)) == 1) {
			return distance(x0, y0, x, y);
		}

		if (x == 0 || y == 0) {
			break;
		}

		e2 = err;

		if (e2 > -dx) {
			err -= dy;
			x += sx;
		}

		if (e2 < dy) {
			err += dx;
			y -= sy;
		}
	}

	return -4.0;
}

double ObstMap::distToObstacleLimited(Pixel_Dimen x0, Pixel_Dimen y0, double angle, double limit) {
	double distance = distToObstacle(x0, y0, angle);

	if (distance <= limit && distance >= 0) {
		return distance;
	} else if (distance == ERR_CODE_COLLISION) {
		return 0.0;
	}

	return limit;
}

Physical_Dimen ObstMap::pixelToPhysDimen(Pixel_Dimen length) {
	return length * PIXEL_PHYSICAL_LENGTH;
}

Pixel_Dimen ObstMap::physToPixelDimen(Physical_Dimen length) {
	return length / PIXEL_PHYSICAL_LENGTH;
}
