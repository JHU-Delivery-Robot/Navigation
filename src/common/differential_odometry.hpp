#ifndef DIFFERENTIAL_ODOMETRY_HPP
#define DIFFERENTIAL_ODOMETRY_HPP

#include <tuple>

#include "vector2.hpp"

namespace common {

/**
 * DifferentialOdometry provides a kinematic model of differential drivetrains
 * for odometry. A given instance tracks the distances the wheels of a robot
 * have driven, and calculates where the robot is based on the wheel distances.
 *
 * Each update to the odometry is fed the current heading of the robot and the
 * total distances the wheels have drive, and uses this to estimate how far and
 * in what direction the robot has driven since the last update. The heading
 * is ideally provided by a gyroscope or compass. However this odometry model
 * is capable of estimating the rotation of the robot since the last update
 * based solely on wheel distances, which can replace a gyroscope if needed.
 *
 * For best results, the odometry should be updated as frequently as feasible.
 * Around 50 times per second will generally give good results. Note that the
 * position estimate provided by odometry will necessarily drift from the
 * robot's actual position over time due to momentary losses of traction and
 * other factors.
 *
 * The units chosen for distance are arbitrary and simply need to be chosen
 * consistently. All angles are in radians, and measured increasing
 * anti-clockwise and starting with zero being the x-axis.
 */
class DifferentialOdometry {
public:
    /**
     * Constructs a model of a specific differential drive train.
     *
     * @param width The wheel base width - the total distance between
     * the left and right wheels. This should be in the same units
     * as the wheel distances will be provided in.
     */
    DifferentialOdometry(double width);

    /**
     * Set the robot's pose, useful for initializing odometry.
     *
     * @param position The position the robot is currently at.
     * @param heading The robot's current heading angle in radians.
     */
    void setPose(common::Vector2 position, double heading);

    /**
     * Get the robot's current pose.
     *
     * @return Position and heading angle in radians.
     */
    std::tuple<common::Vector2, double> getPose();

    /**
     * Update the estimate of the robot's pose using the kinematic model.
     * For best results, update odometry frequently.
     *
     * @param heading The current heading angle of the robot.
     * @param total_left_distance Total distance driven by left wheels.
     * @param total_right_distance Total distance driven by right wheels.
     */
    void updateOdometry(double heading, double total_left_distance, double total_right_distance);

    /**
     * Estimate the rotation of the robot since the last odometry update, based
     * on the distances the wheels have driven. This does not change the estimated
     * pose at all.
     *
     * Provided as an alternative method of tracking heading if a gyroscopic sensor
     * or compass is unavailable, or for modelling heading changes in a simulation.
     * The estimated rotation can simply be added to a running estimate of the robot's
     * heading before each call to updateOdometry. If this method is chosen rather
     * than a gyroscope, the estimated heading will drift over time.
     *
     * @param total_left_distance Total distance driven by left wheels.
     * @param total_right_distance Total distance driven by right wheels.
     */
    double estimateRotation(double total_left_distance, double total_right_distance) const;

private:
    double wheel_base_width;

    common::Vector2 position;
    double heading;
    double left_wheels_distance, right_wheels_distance;
};

}  // namespace common

#endif
