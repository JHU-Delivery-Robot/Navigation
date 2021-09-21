#include "catch2/catch.hpp"

#include <memory>
#include <string>

#include "sensor_gen.hpp"
#include "obstacle_map.hpp"
#include "vector2.hpp"

using namespace common;
using namespace sim;

TEST_CASE("Ultrasonic Sensor Test", "[Ultrasonic][SensorGen]")
{
   Polygon polygon = Polygon({Vector2(1.0, 1.0), Vector2(2.0, 1.0), Vector2(2.0, -1.0), Vector2(1.0, -1.0)});
   ObstacleMap map = ObstacleMap({polygon});

   BeamModel beam = BeamModel(0.0, 0.0, 300.0, 0.0, 0.0);
   DistanceSensorModel model = DistanceSensorModel(map, beam, 300.0);

   UltrasonicSensorImpl impl = UltrasonicSensorImpl(model, 0.1 * PI, 5);
   UltrasonicSensor *ultrasonic = &impl;

   impl.updateLocation(Vector2(-1.0, 0.0), 0.0);
   CHECK(ultrasonic->read() == Approx(2.0).epsilon(0.001));

   impl.updateLocation(Vector2(-1.0, 0.0), PI);
   CHECK(ultrasonic->read() == Approx(300.0).epsilon(0.001));

   impl.updateLocation(Vector2(-1.0, 0.0), 0.5 * PI);
   CHECK(ultrasonic->read() == Approx(300.0).epsilon(0.001));

   impl.updateLocation(Vector2(-250.0, 0.0), 0.0);
   CHECK(ultrasonic->read() == Approx(251.0).epsilon(0.001));

   impl.updateLocation(Vector2(-305, 0.0), 0.0);
   CHECK(ultrasonic->read() == Approx(300.0).epsilon(0.001));
}
