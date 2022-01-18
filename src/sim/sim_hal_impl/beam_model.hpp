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
     * @param lambda parameter for exponential distribution
     * @param max_reading maximum reading of sensor
     * @param w_exp weight of exponential component
     * @param w_rand wight of the uniform component
     */
    BeamModel(double std_dev, double lambda, double max_reading,
                double w_exp, double w_rand);
    
    /*
    * Sample sensor reading based on specified sensor statistics and actual distance to obstacle
    */
    double sample(double actualDistance);

private:
    double sampleGlitch();
    double sampleNormal(double actualDistance);
    bool glitch();

    double std_dev;     /**< std deviation of the gaussian sensor dist. (accuracy) */
    double max_reading; /**< recip. prob for the unifgorm dist. used for glitches. This
                        is also the maximum possible reading */
    // probability weight of each of the 4 cases
    double p_exp; /**< probability of the exponential component */
    //double p_hit;  /**< probability of the gaussian component */
    //double p_max;  /**< probability of getting a max value reading*/
    double p_rand; /**< probability of the uniform distribution */

    std::mt19937 gen;
    std::exponential_distribution<double> exp_dist;
    std::uniform_real_distribution<double> uniform_dist_rand;
};

}

#endif
