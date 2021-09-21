#include "beam_model.hpp"

namespace sim {

BeamModel::BeamModel(double std_dev, double lambda, double max_reading,
                     double w_exp, double w_rand)
    : std_dev(std_dev), max_reading(max_reading),
      p_exp(w_exp), p_rand(w_rand),
      exp_dist(lambda), uniform_dist_rand(0,1) {
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

double BeamModel::sampleGlitch() {
    if (uniform_dist_rand(gen)*(p_exp + p_rand) < p_exp) {
        return exp_dist(gen);
    } else {
        return uniform_dist_rand(gen)*max_reading;
    }
}

double BeamModel::sample(double actualDistance) {
    return glitch() ? sampleGlitch() : sampleNormal(actualDistance);
}

bool BeamModel::glitch() {
    return uniform_dist_rand(gen) < (p_exp + p_rand);
}

}
