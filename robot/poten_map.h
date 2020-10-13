/* 
 * JHU Deliverbot Navigation Group 
 * Navigation simulation system 
 * Potential maps --- header file 
 *
 * @avnoks (GitHub)  23 Sep 2020
 *
 */

/* A NOTICE (23 Sep 2020): We're at an early stage.  Feel free to change 
 * things like function names and types to things that make more sense.
 */

#include <stddef.h>
#include <stdint.h>

/* ----------------------------------------------------------------------
 *   Defines, typedefs
 * ---------------------------------------------------------------------- */

/* These might change later */
#define N_ANGLES 360
#define N_DISTANCES 500
#define Q_STAR_REPULSIVE 20
#define Q_STAR_TRENCH 10

typedef uint32_t Radius; 
typedef int16_t Angle;
typedef int16_t Cart_dist; // used for cartesian coordinates!

/* Number that represents potential at particular location in map */
typedef uint16_t Potential;     /* Maybe we want to use floats instead? */
typedef struct {
  Potential *map;               /* Array of potentials */
  unsigned n_angles;
  unsigned n_distances;
} Potential_Map;

/* Don't use this vvv --- use the above struct */
// potential_t Map[N_ANGLES][N_DISTANCES];

typedef struct {
  Cart_dist x;
  Cart_dist y;
  Radius radius;
  Angle angle;
} Point;

typedef struct {
  Point start;
  Point goal;
  Point r0;
  double gamma; // radial line purpendicular to path in radians
} Polar_Path;



/* ----------------------------------------------------------------------
 *   Function prototypes
 * ---------------------------------------------------------------------- */

/* 
 * Allocates a potential map with default dimensions 
 * (number of discrete angles: N_ANGLES, 
 *  number of discrete distances: N_DISTANCES)
 * Returns: 
 *   Ptr to freshly allocated potential map struct (must be freed)
 */
Potential_Map *allocate_default_potential_map();

/* Frees potential map. */
void destroy_potential_map(Potential_Map *map);


/* Computes squared euclidean distance between two points in 
 * polar coordinates  
 * Args: 
 *   r1      -  radius of first pt
 *   theta1  -  angle to first pt
 *   r2      -  radius of second pt
 *   theta2  -  angle to second pt
 * Returns: 
 *   double representing SQUARED euclidean distance b.w. two pts.
 */
double euclidean_dist_squared(Radius r1, Angle theta1, Radius r2, Angle theta2);

/* 
 * Computes attractive potential at a particlar coordinate given location 
 * of goal.  
 * Args: 
 *   r          - radius of pt to calculate potential at
 *   theta      - angle to pt to calculate potential at
 *   goal_r     - radius of goal pt
 *   goal_theta - angle to goal pt
 * Returns: 
 *   attractive potential at specified point
 *
 */
Potential attractive_potential(Radius r, Angle theta, Radius goal_r, Angle goal_theta);

/* 
 * Computes repulsive potential at a particlar radius given radius of 
 * obstacle.  It is assumed that the angle to the obstacle is known but 
 * taken care of elsewhere.
 * Args: 
 *   r          - radius of pt to calculate potential at
 *   obstacle_r     - radius of goal pt
 * Returns: 
 *   repulsive potential at specified radius
 *
 */
Potential repulsive_potential(Radius r, Radius obstacle_r);

/* 
 * Computes trench potential at a given perpendicular distance from path
 * Args: 
 *   d - perpendicular distance to path
 * Returns: 
 *   trench potential at specified distance from path
 */
Potential trench_potential(Radius d);


/*
 * Wrapper around accessing potential map to handle out-of-boungs
 * angle values
 * Args:
 *    pmap  - pointer to Potential_Map struct
 *    a     - angle of the location to edit potential for
 *    r     - radius of the location to edit potential for
 *    poten - potential value to add to the map
 */
void edit_potential_map(Potential_Map *pmap, Angle a, Radius r, Potential poten);


/* 
 * Given a map and the polar coordinates of a goal point, apply 
 * an attractive potential to the entire map.
 *   --> Iterate through map and call attractive_potential() .
 * Args: 
 *   pmap       - ptr to potential map
 *   goal_r     - radius of goal pt
 *   goal_theta - angle to goal pt
 */
void apply_attr_poten(Potential_Map *pmap, Radius goal_r, Angle goal_theta);


/* 
 * Given a map and the polar coordinates of a goal point, apply 
 * an attractive potential to the entire map.
 *   --> Iterate through LIDAR data and call repulsive_potential() 
 *       on appropriate radii.
 * Args: 
 *   pmap       - ptr to potential map
 *   lidar_data - Array of distances to obstacle indexed by angle
 */
void apply_repulsive_poten(Potential_Map *pmap, Radius* lidar_data);


/*
 * Given 2 points in cartesian coordinates, finds the slope
 * between them
 * NOTE: assumed that x1 != x2 and y1 != y2
 */
double find_slope(Cart_dist x1, Cart_dist y1, Cart_dist x2, Cart_dist y2);


/*
 * An arctan wrapper function, handling zero division 
 * and out-of-range angles
 * Args:
 *    x_val - x value of the point we're taking arctan of
 *    y_val - y value of the point we're taking arctan of
 *    num_angles - the number of angles on the relevant potential map
 * Returns:
 *    Angle of the given point, scaled to the given number of angles
 */
Angle arctan(Cart_dist x_val, Cart_dist y_val, uint16_t num_angles);


/*
 * Given cartesian endpoints of the desired path, calculates
 * info necessary to define that path in polar and stores in in
 * a struct
 * Args:
 *    start_point  -  Point where path begins
 *    goal_point  -  Point where path ends
 *    num_angles  -  total angles for this polar potential map (ie 360, 720)
 * Returns:
 *     pointer to Polar_Path struct
 */
Polar_Path * create_polar_path(Point start_point, Point goal_point, uint16_t num_angles);


/*
 * Given the cartesian endpoints of the path, finds the angle gamma of
 * the radial line perpendicular to the path
 * Can assume that slope of path is defined (not 0 or inf)
 * Args:
 *     x1  -  x-coordinate of starting point
 *     y1  -  y-coordinate of starting point
 *     x2  -  x-coordinate of goal
 *     y2  -  y-coordinate of goal
 *     num_angles  -  total angles for this polar potential map (ie 360, 720)
 * Returns:
 *    Angle gamma of the radial line (in radians)
 */
double radial_line(Cart_dist x1, Cart_dist y1, Cart_dist x2, Cart_dist y2);


/*
 * Given the cartestian endpoints of the path, find the point at
 * which the path intersects the radial line. This is done by
 * setting up a system of linear equations and solving.
 * NOTE: assume that neither line is horizontal or vertical
 *       (slopes will not be 0 or infinity, so divide freely)
 *
 */
Radius find_intersection(Cart_dist x1, Cart_dist y1, Cart_dist x2, Cart_dist y2);

/*
 * Given a the polar representation of the path and the potential map,
 * calculate and apply the trench potential
 * Trench potential applies to points directly on the path, as well as those
 * within distance Q_STAR_TRENCH perpendicular to the path
 * Args:
 *    path  -  pointer to Polar_Path struct containing necessary info
 *    pmap  -  pointer to Potential_Map where potentials will be added
 *    num_angles - total angles this polar potential map (ie 360, 720)
 *
 */
void apply_trench_poten(Polar_Path * path, Potential_Map *pmap, uint16_t num_angles);


/* 
 * Beware of bugs in the above code; I have only proved it correct, not 
 * tried it. 
 *                                    --- Donald K. Knuth
 */
