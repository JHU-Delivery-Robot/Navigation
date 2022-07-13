#ifndef BEAM_MODEL_HPP
#define BEAM_MODEL_HPP

#include <random>

namespace sim
{

class BeamModel
{
public:
    /**
     * @param map obstacle map to use to intialize values
     * @param std_dev standard deviation fo sensor gaussian
     * @param max_reading maximum reading of sensor
     * @param w_max weight of maximum component
     * @param w_rand wight of the uniform component
     */
    BeamModel(double std_dev, double max_reading,
                double w_max, double w_rand);
    
    /*
    * Sample sensor reading based on specified sensor statistics and actual distance to obstacle
    */
    double sample(double actualDistance);

private:
    double sampleMax();
    double sampleRandom();
    double sampleNormal(double actualDistance);

    double std_dev;     /**< std deviation of the gaussian sensor dist. (accuracy) */
    double max_reading; /**< recip. prob for the unifgorm dist. used for glitches. This
                        is also the maximum possible reading */
    // probability weight of each of the 3 cases
    double p_max; /**< probability of the maximum component */
    double p_rand; /**< probability of the uniform distribution */
    //double p_hit;  /**< probability of the gaussian component */

    std::mt19937 gen;
    std::uniform_real_distribution<double> uniform_dist_rand;
};

}

#endif
