#include <tuple>

#include "catch2/catch.hpp"
#include "common.hpp"
#include "differential_odometry.hpp"

using namespace common;

TEST_CASE("DifferentialOdometry EstimateRotation", "[DifferentialOdometry]") {
    static constexpr double radius = 2.0;
    DifferentialOdometry odometry = DifferentialOdometry(2 * radius);

    // Driving straight
    CHECK(odometry.estimateRotation(2.0, 2.0) == Approx(0.0));
    CHECK(odometry.estimateRotation(-2.0, -2.0) == Approx(0.0));
    CHECK(odometry.estimateRotation(0.0, 0.0) == Approx(0.0));

    // Turning in place
    CHECK(odometry.estimateRotation(-0.5 * PI * radius, 0.5 * PI * radius) == Approx(0.5 * PI).margin(1e-6));
    CHECK(odometry.estimateRotation(0.5 * PI * radius, -0.5 * PI * radius) == Approx(-0.5 * PI).margin(1e-6));
    CHECK(odometry.estimateRotation(-0.1 * PI * radius, 0.1 * PI * radius) == Approx(0.1 * PI).margin(1e-6));
    CHECK(odometry.estimateRotation(0.1 * PI * radius, -0.1 * PI * radius) == Approx(-0.1 * PI).margin(1e-6));

    /// Turning while driving forward/backward
    CHECK(odometry.estimateRotation(0.25 * PI * 3.0, 0.25 * PI * (3.0 + 2 * radius)) == Approx(0.25 * PI).margin(1e-6));
    CHECK(odometry.estimateRotation(0.25 * PI * (3.0 + 2 * radius), 0.25 * PI * 3.0) == Approx(-0.25 * PI).margin(1e-6));
    CHECK(odometry.estimateRotation(-0.25 * PI * 3.0, -0.25 * PI * (3.0 + 2 * radius)) == Approx(-0.25 * PI).margin(1e-6));
    CHECK(odometry.estimateRotation(-0.25 * PI * (3.0 + 2 * radius), -0.25 * PI * 3.0) == Approx(0.25 * PI).margin(1e-6));

    CHECK(odometry.estimateRotation(0.05 * PI * 3.0, 0.05 * PI * (3.0 + 2 * radius)) == Approx(0.05 * PI).margin(1e-6));
    CHECK(odometry.estimateRotation(0.05 * PI * (3.0 + 2 * radius), 0.05 * PI * 3.0) == Approx(-0.05 * PI).margin(1e-6));
    CHECK(odometry.estimateRotation(-0.05 * PI * 3.0, -0.05 * PI * (3.0 + 2 * radius)) == Approx(-0.05 * PI).margin(1e-6));
    CHECK(odometry.estimateRotation(-0.05 * PI * (3.0 + 2 * radius), -0.05 * PI * 3.0) == Approx(0.05 * PI).margin(1e-6));

    // Turn radius inside of drivetrain, close to turning in place but not quite
    CHECK(odometry.estimateRotation(-0.25 * PI * 4 / 3, 2 * 0.25 * PI * 4 / 3) == Approx(0.25 * PI).margin(1e-6));
    CHECK(odometry.estimateRotation(2 * 0.25 * PI * 4 / 3, -0.25 * PI * 4 / 3) == Approx(-0.25 * PI).margin(1e-6));
    CHECK(odometry.estimateRotation(-0.05 * PI, 3 * 0.05 * PI) == Approx(0.05 * PI).margin(1e-6));
    CHECK(odometry.estimateRotation(3 * 0.05 * PI, -0.05 * PI) == Approx(-0.05 * PI).margin(1e-6));
}

TEST_CASE("DifferentialOdometry UpdateOdometry", "[DifferentialOdometry]") {
    static constexpr double radius = 2.0;
    DifferentialOdometry odometry = DifferentialOdometry(2 * radius);

    common::Vector2 position(0.0, 0.0);
    double heading = 0;

    odometry.setPose(position, heading);

    double left_wheel_distance = 0.0;
    double right_wheel_distance = 0.0;

    // Drive straight forward
    left_wheel_distance += 1.0;
    right_wheel_distance += 1.0;
    odometry.updateOdometry(0.0, left_wheel_distance, right_wheel_distance);
    std::tie(position, heading) = odometry.getPose();
    CHECK(heading == Approx(0.0));
    CHECK(position.x == Approx(1.0));
    CHECK(position.y == Approx(0.0));

    // Then backwards
    left_wheel_distance -= 3.0;
    right_wheel_distance -= 3.0;
    odometry.updateOdometry(0.0, left_wheel_distance, right_wheel_distance);
    std::tie(position, heading) = odometry.getPose();
    CHECK(heading == Approx(0.0));
    CHECK(position.x == Approx(-2.0));
    CHECK(position.y == Approx(0.0));

    // Now turn in place to the left
    left_wheel_distance -= 2.0;
    right_wheel_distance += 2.0;
    odometry.updateOdometry(0.5 * PI, left_wheel_distance, right_wheel_distance);
    std::tie(position, heading) = odometry.getPose();
    CHECK(heading == Approx(0.5 * PI));
    CHECK(position.x == Approx(-2.0));
    CHECK(position.y == Approx(0.0));

    // Then to the right
    left_wheel_distance += 4.0;
    right_wheel_distance -= 4.0;
    odometry.updateOdometry(-0.5 * PI, left_wheel_distance, right_wheel_distance);
    std::tie(position, heading) = odometry.getPose();
    CHECK(heading == Approx(-0.5 * PI));
    CHECK(position.x == Approx(-2.0));
    CHECK(position.y == Approx(0.0));

    // Now drive forward while turning left
    left_wheel_distance += 0.5 * PI * (3 - radius);
    right_wheel_distance += 0.5 * PI * (3 + radius);
    odometry.updateOdometry(0.0, left_wheel_distance, right_wheel_distance);
    std::tie(position, heading) = odometry.getPose();
    CHECK(heading == Approx(0.0));
    CHECK(position.x == Approx(1.0));
    CHECK(position.y == Approx(-3.0));

    // Then backwards while turning left
    left_wheel_distance += PI * (20 + radius);
    right_wheel_distance += PI * (20 - radius);
    odometry.updateOdometry(-PI, left_wheel_distance, right_wheel_distance);
    std::tie(position, heading) = odometry.getPose();
    CHECK(heading == Approx(-PI));
    CHECK(position.x == Approx(1.0));
    CHECK(position.y == Approx(-43.0));
}
