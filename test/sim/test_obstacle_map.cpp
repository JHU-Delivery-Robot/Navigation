#include "catch2/catch.hpp"

#include "obstacle_map.hpp"

using namespace sim;
using namespace common;

constexpr double PI = 3.14159265358979323846;

TEST_CASE("Obstacle Map No Obstacles", "[ObstacleMap]")
{
    sim::ObstacleMap map = ObstacleMap(std::vector<Polygon>());

    bool obstacle_found;
    double distance;

    std::tie(obstacle_found, distance) = map.distanceToObstacle(Vector2(0.0, 0.0), Vector2::polar(0.0, 1.0));
    REQUIRE(obstacle_found == false);

    std::tie(obstacle_found, distance) = map.distanceToObstacle(Vector2(0.0, 0.0), Vector2::polar(0.5 * PI, 1.0));
    REQUIRE(obstacle_found == false);

    std::tie(obstacle_found, distance) = map.distanceToObstacle(Vector2(0.0, 0.0), Vector2::polar(1.0 * PI, 1.0));
    REQUIRE(obstacle_found == false);

    std::tie(obstacle_found, distance) = map.distanceToObstacle(Vector2(0.0, 0.0), Vector2::polar(1.5 * PI, 1.0));
    REQUIRE(obstacle_found == false);
}

TEST_CASE("Obstacle Map No Intersections", "[ObstacleMap]")
{
    Polygon polygon = Polygon({Vector2(1.0, 1.0), Vector2(2.0, 1.0), Vector2(2.0, -1.0), Vector2(1.0, -1.0)});
    ObstacleMap map = ObstacleMap({polygon});

    bool obstacle_found;
    double distance;

    std::tie(obstacle_found, distance) = map.distanceToObstacle(Vector2(0.0, 0.0), Vector2::polar(0.5 * PI, 1.0));
    REQUIRE(obstacle_found == false);

    std::tie(obstacle_found, distance) = map.distanceToObstacle(Vector2(0.0, 0.0), Vector2::polar(-0.5 * PI, 1.0));
    REQUIRE(obstacle_found == false);

    std::tie(obstacle_found, distance) = map.distanceToObstacle(Vector2(0.0, 0.0), Vector2::polar(PI, 1.0));
    REQUIRE(obstacle_found == false);

    std::tie(obstacle_found, distance) = map.distanceToObstacle(Vector2(0.0, 0.0), Vector2::polar(0.3 * PI, 1.0));
    REQUIRE(obstacle_found == false);
}

TEST_CASE("Obstacle Map Simple Intersections", "[ObstacleMap]")
{
    Polygon polygon = Polygon({Vector2(1.0, 1.0), Vector2(2.0, 1.0), Vector2(2.0, -1.0), Vector2(1.0, -1.0)});
    ObstacleMap map = ObstacleMap({polygon});

    bool obstacle_found;
    double distance;

    std::tie(obstacle_found, distance) = map.distanceToObstacle(Vector2(0.0, 0.0), Vector2::polar(0.0, 3.5));
    CHECK(obstacle_found);
    CHECK(distance == Approx(1.0).epsilon(1e-6));

    std::tie(obstacle_found, distance) = map.distanceToObstacle(Vector2(0.0, 0.0), Vector2::polar(0.2 * PI, 2.0));
    CHECK(obstacle_found);
    CHECK(distance == Approx(sqrt(1 + tan(0.2 * PI) * tan(0.2 * PI))).epsilon(1e-6));

    polygon = Polygon({Vector2(1.0, 1.0), Vector2(1.0, -1.0), Vector2(2.0, -1.0), Vector2(2.0, 1.0)});
    map = ObstacleMap({polygon});

    std::tie(obstacle_found, distance) = map.distanceToObstacle(Vector2(0.0, 0.0), Vector2::polar(0.0, 2.0));
    CHECK(obstacle_found);
    CHECK(distance == Approx(1.0).epsilon(1e-6));

    std::tie(obstacle_found, distance) = map.distanceToObstacle(Vector2(0.0, 0.0), Vector2::polar(0.2 * PI, 2.0));
    CHECK(obstacle_found);
    CHECK(distance == Approx(sqrt(1 + tan(0.2 * PI) * tan(0.2 * PI))).epsilon(1e-6));
}
