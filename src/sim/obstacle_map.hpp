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

#ifndef OBSTACLE_MAP_HPP
#define OBSTACLE_MAP_HPP

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

/* ----------------------------------------------------------------------
 *   Defines, typedefs
 * ---------------------------------------------------------------------- */

#define PI 3.14159265358979323

typedef uint8_t Obst_Map_Px;
typedef unsigned Pixel_Dimen;
typedef unsigned Physical_Dimen;

/* ----------------------------------------------------------------------
 *   Function and Class prototypes
 * ---------------------------------------------------------------------- */

/* Class for storing obstacle map and relevant information */
class ObstMap {
protected:

	std::vector<Obst_Map_Px> map;
	Pixel_Dimen width;
	Pixel_Dimen height;

	Pixel_Dimen CoordsToBitmapIndex(Pixel_Dimen x, Pixel_Dimen y);

	double distance(Pixel_Dimen x0, Pixel_Dimen y0, Pixel_Dimen x1, Pixel_Dimen y1);

public:

	/*
	 * Allocates an Obst_Map and fills it with data from bitmap stored at pathname
	 * Args: 
	 *   pathname  -  Pathname of bitmap file
	 * Returns:
	 *   ptr to freshly allocated Obst_Map containing data in loaded bitmap 
	 *      (you must free this pointer when you are done with the map)
	 */
	ObstMap(std::string pathname);

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
	double distToObstacle(Pixel_Dimen x0, Pixel_Dimen y0, double angle);

	/*
	 * Computes distance to nearest obstacle within a specified distance 
	 * from a given point in a given direction.
	 * Args: 
	 *   x      - x coordinate of point to start searching from
	 *   y      - y coordinate of point to start searching from
	 *   angle  - direction to search for obstacles in radians
	 *   limit  - distance, in pixel units, to stop searching at
	 * Returns: 
	 *   double float (for some semblance of precision) of distance 
	 *     in pixel units from given point to nearest obstacle IF
	 *     obstacle was found within limit.  OTHERWISE, returns -1.0d0 
	 *     to indicate that no obstacle was found within limit.
	 */
	double distToObstacleLimited(Pixel_Dimen x0, Pixel_Dimen y0, double angle, double limit);

	/* Functions to convert between pixel lengths and corresponding physical 
 	* lengths and vice versa.  Should we inline these? */
	Physical_Dimen pixelToPhysDimen(Pixel_Dimen length);
	Pixel_Dimen physToPixelDimen(Physical_Dimen length);
};

#endif