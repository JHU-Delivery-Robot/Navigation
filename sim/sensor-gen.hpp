/*
 * JHU Deliverbot Navigation Group 
 * Navigation simulation system 
 * Generate sensor data for simulation --- header file 
 *
 * @avnoks (GitHub)  5 Jan 2021 (create date)
 *
 */

/* A NOTICE (5 Jan 2020): We're at an early stage.  Feel free to change 
 * things like function names and types to things that make more sense.
 */

#ifndef SENSOR_GEN

#define SENSOR_GEN
/* Need to include obstacle map header file */ //#include ""

typedef unsigned Length;
typedef unsigned Angle;

class BeamModel {
public:
	double std_dev;
	double lambda;
	double max_reading;

	double w_exp;
	double w_hit;
	double w_max;
	double w_rand;

	/* 
	 * Sample sensor reading from distribution described by statistics.
	 * Arguments:
	 *   dist   -  true distance to obstacle
	 * Returns: 
	 *   double float sampled from distribution
	 */
	Length sample(Length dist);
};

class Lidar {
private:
	BeamModel beam;

public:
	/* 
	 * Generate Lidar readings and write to array. (Use BeamModel in lieu of sensor statistics)
	 * Arguments
	 *   readings - pointer to array to write Lidar readings to
	 *   map      - obstacle map reference
	 *   x        - x coordinate of lidar in obstacle map
	 *   y        - y coordinate of lidar in obstacle map
	 * Returns squat
	 */
	void generate(Length *readings, Obst_Map& map, Length x, Length y);
};

class UltrasoundSensor {
private:
	BeamModel beam;

public:
	/* 
	 * Generate ultrasound reading. (Use BeamModel in lieu of sensor statistics)
	 * Arguments
	 *   readings - pointer to array to write Lidar readings to
	 *   map      - obstacle map reference
	 *   x        - x coordinate of sensor in obstacle map
	 *   y        - y coordinate of sensor in obstacle map
	 *   heading  - angle in which the sensor is pointed
	 */
	Length generate(Length *readings, Obst_Map& map, Length x, Length y);
};

class IRSensor {
private:
	BeamModel beam;

public:
	/* 
	 * Generate ultrasound reading. (Use BeamModel in lieu of sensor statistics)
	 * Arguments
	 *   map      - obstacle map reference
	 *   x        - x coordinate of sensor in obstacle map
	 *   y        - y coordinate of sensor in obstacle map
	 *   heading  - angle in which the sensor is pointed
	 */
	Length IR_sensor(Obst_Map& map, Length x, Length y, Angle heading);
};

//void lidar(Length *readings, Sensor_stats stats, Obst_Map map, Length x, Length y);

//Length ultrasound_sensor(Sensor_stats stats, Obst_Map map, Length x, Length y, Angle heading);

/* Implement this last.  :| */
//Length IR_sensor(Sensor_stats stats, Length x, Length y, Angle heading);

#endif
