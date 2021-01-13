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

#include <cstddef>
#include <cstdint>

/* ----------------------------------------------------------------------
 *   Defines, typedefs
 * ---------------------------------------------------------------------- */

namespace std {
	class string;
	class vector;
};

#define PI 3.14159265358979323

/*typedef uint8_t Obst_Map_Px;
typedef unsigned Pixel_Dimen;
typedef unsigned Physical_Dimen;*/

/* ----------------------------------------------------------------------
 *   Function and Class prototypes
 * ---------------------------------------------------------------------- */

/* Class for storing obstacle map and relevant information */
class Obst_Map {
private:

	std::vector<uint8_t> map;
	unsigned width;
	unsigned height;

	unsigned spatial_transform(unsigned x_robot, unsigned y_robot, int x, int y);

public:

	/*
	 * Allocates an Obst_Map and fills it with data from bitmap stored at pathname
	 * Args: 
	 *   pathname  -  Pathname of bitmap file
	 * Returns:
	 *   ptr to freshly allocated Obst_Map containing data in loaded bitmap 
	 *      (you must free this pointer when you are done with the map)
	 */
	Obst_Map(std::string pathname);

	unsigned get_height();
	
	unsigned get_weight();

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
	double dist_to_obstacle(unsigned x, unsigned y, double angle);

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
	double dist_to_obstacle_limited(unsigned x, unsigned y, double angle, unsigned limit);

	/* Functions to convert between pixel lengths and corresponding physical 
 	* lengths and vice versa.  Should we inline these? */
	unsigned pixel_to_phys_dimen(unsigned length);
	unsigned phys_to_pixel_dimen(unsigned length);
};

#endif