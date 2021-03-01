/**
 * @file sensor-gen.hpp
 * JHU Deliverbot Navigation Group
 * Navigation simulation system
 * Generate sensor data for simulation --- header file
 *
 * @author avnoks (GitHub)  5 Jan 2021 (create date)
 * @author jocelynhsu8
 */

/* A NOTICE (5 Jan 2020): We're at an early stage.  Feel free to change
 * things like function names and types to things that make more sense.
 */

#ifndef SENSOR_GEN

#define SENSOR_GEN
/* Need to include obstacle map header file */ //#include ""
// forward declare obstaclemap type
class ObstMap;
// typedefs
typedef double Length;
//RADIANS
typedef double Angle;

#define ANGLE_INCREMENT 0.00872664625
#define PI 3.14159265358979323

/**
 * Class that defines a beammodel
 *
 * add paragraph about statistical engine calculation
 * Note that the trivial copy construuctor/assignment operator is used which
 * will assign fields element by element. If additional pointer are added as
 * fields, this will need to be overridden
 */
class BeamModel {
private:
    double std_dev;     /**< std deviation of the gaussian sensor dist. (accuracy) */
    double lambda;      /**< parameter for an exponential dist. of short reading */
    double max_reading; /**< recip. prob for the unifgorm dist. used for glitches. This
                             is also the maximum possible reading */
    // probability weight of each of the 4 cases
    double p_exp;  /**< probability of the exponential component */
    //double p_hit;  /**< probability of the gaussian component */
    //double p_max;  /**< probability of getting a max value reading*/
    double p_rand; /**< probability of the uniform distribution */

public:
    /**
     * Constructor to create a new BeamModel
     *
     * @param map obstacle map to use to intialize values
     * @param std_dev standard deviation fo sensor gaussian
     * @param lambda parameter for exponential distribution
     * @param max_reading maximum reading of sensor
     * @param w_exp weight of exponential component
     * @param w_rand wight of the uniform component
     */
    BeamModel(double std_dev, double lambda, double max_reading,
              double w_exp, double w_rand);
    /*
     * Sample sensor reading from distribution described by statistics.
     * @param dist true distance to obstacle
     * @return float sampled from distribution
     */
    Length sampleNormal(Length dist); // 
    Length sampleGlitch(Length dist); // obstacle map --> within range, within dist + gaussian noise; else max value
    bool glitch(); // call first -> if T call sampleGlitch else call sampleNormal; ultracount sampleNorm 15 deg, Lidar 720 deg
};

/**
 * Class that defines a beammodel (edit for sensorgen)
 *
 * Note that the trivial copy construuctor/assignment operator is used which
 * will assign fields element by element. If additional pointer are added as
 * fields, this will need to be overridden
 */
class SensorGen {
protected:
    BeamModel beam; /**< internal beammodel used by the sensor */
    ObstMap& map;  /**< obstacle map referenced by the sensor */
public:
    SensorGen(ObstMap& map, BeamModel& mdl);
};

/**
 * Cone Sensor model
 */
class ConeSensor : public SensorGen {
protected:
    Angle fov;  /**< obstacle map referenced by the sensor */
public:
    ConeSensor(ObstMap& map, Angle fov, BeamModel& mdl);
    /**
     * Generate ultrasound reading. (Use BeamModel in lieu of sensor statistics)
     *   @param x x coordinate of sensor in obstacle map
     *   @param y y coordinate of sensor in obstacle map
     *   @param fov size of ultrasonic "cone (can round to nearest half angle)
     *   @param heading angle in which the sensor is pointed (center of fov)
     *   @return simulated distance reading from sensor
     */
    Length generate(Length x, Length y, Angle heading);
};

/**
 * Lidar model
 */
class Lidar : public SensorGen {
public:
    Lidar(ObstMap& map, BeamModel& mdl);
    /**
     * Generate Lidar readings and write to array. (Use BeamModel in lieu of sensor statistics)
     * @param readings pointer to array to write Lidar readings to
     * @param x x coordinate of lidar in obstacle map
     * @param y y coordinate of lidar in obstacle map
     */
    void generate(Length *readings, Length x, Length y);
};

/**
 * Ultrasonic sensor model
 */
class UltrasoundSensor : public ConeSensor {
public:
    UltrasoundSensor(ObstMap& map, Angle fov, BeamModel& mdl);
    /**
     * Generate ultrasound reading. (Use BeamModel in lieu of sensor statistics)
     *   @param x x coordinate of sensor in obstacle map
     *   @param y y coordinate of sensor in obstacle map
     *   @param fov size of ultrasonic "cone (can round to nearest half angle)
     *   @param heading angle in which the sensor is pointed (center of fov)
     *   @return simulated distance reading from sensor
     */
    Length generate(Length x, Length y, Angle heading);
};

/**
 * IR sensor model
 */
class IRSensor : public ConeSensor {
public:
    IRSensor(ObstMap& map, Angle fov, BeamModel& mdl);
    /**
     * Generate ultrasound reading. (Use BeamModel in lieu of sensor statistics)
     *   @param map obstacle map reference
     *   @param x x coordinate of sensor in obstacle map
     *   @param y y coordinate of sensor in obstacle map
     *   @param heading  - angle in which the sensor is pointed (center of fov)
     *   @return simulated distance reading from sensor
     */
    Length generate(Length x, Length y, Angle heading);
};

#endif