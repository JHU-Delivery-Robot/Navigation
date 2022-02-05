#include "beam_model.hpp"

namespace sim {

BeamModel::BeamModel(double std_dev, double max_reading,
                     double w_max, double w_rand)
    : std_dev(std_dev), max_reading(max_reading),
      p_max(w_max), p_rand(w_rand),
      uniform_dist_rand(0,1) {
        std::random_device random_device;
        gen.seed(random_device());
}

double BeamModel::sampleNormal(double actualDistance) {
    std::normal_distribution<double> norm_dist(actualDistance, std_dev);
    double rand_norm = norm_dist(gen);

    if (rand_norm < max_reading) {
        return rand_norm;
    } else {
        return max_reading;
    }
}

double BeamModel::sampleRandom() {
    return uniform_dist_rand(gen) * max_reading;
}

double BeamModel::sampleMax() {
    return max_reading;
}

double BeamModel::sample(double actualDistance) {
        double p = uniform_dist_rand(gen);
    if (p < p_rand) {
        return sampleRandom();
    }
    else if (p < p_rand + p_max) {
        return sampleRandom();
    }
    else {
        return sampleNormal(actualDistance);
    }
}

}