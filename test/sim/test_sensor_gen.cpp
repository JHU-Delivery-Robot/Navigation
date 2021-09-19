#include <string>

#include "catch.hpp"
#include "stb_image_write.h"

#include "sensor-gen.hpp"
#include "obstacle_map.hpp"

using std::string;

#define COORD_TO_IN(c, r, n_c) (r * n_c + c)

#define degreesToRadians(angleDegrees) (angleDegrees * PI / 180.0)
#define radiansToDegrees(angleRadians) (angleRadians * 180.0 / PI)

/**
 * Verify that a map matches an input image
 * @param map obstable map to check
 * @param img_data image to compare against
 */
static string generate_image()
{
   string test_map_img = "omap_distance_test.png";
   size_t data_size = 600 * 600;
   char *data = new char[data_size];

   for (size_t i = 0; i < data_size; ++i)
   {
      data[i] = static_cast<char>(0);
   }

   //Edge Case 1
   data[COORD_TO_IN(0, 0, 600)] = static_cast<char>(255);
   data[COORD_TO_IN(0, 1, 600)] = static_cast<char>(255);

   //Edge Case 1
   data[COORD_TO_IN(599, 0, 600)] = static_cast<char>(255);
   data[COORD_TO_IN(599, 1, 600)] = static_cast<char>(255);

   //Quadrant 1
   data[COORD_TO_IN(379, 250, 600)] = static_cast<char>(255);
   data[COORD_TO_IN(500, 200, 600)] = static_cast<char>(255);

   //Quadrant 2
   data[COORD_TO_IN(140, 150, 600)] = static_cast<char>(255);
   data[COORD_TO_IN(200, 50, 600)] = static_cast<char>(255);

   //Quadrant 3
   data[COORD_TO_IN(50, 420, 600)] = static_cast<char>(255);
   data[COORD_TO_IN(290, 500, 600)] = static_cast<char>(255);

   //Quadrant 4
   data[COORD_TO_IN(315, 435, 600)] = static_cast<char>(255);
   data[COORD_TO_IN(550, 317, 600)] = static_cast<char>(255);

   stbi_write_png(test_map_img.c_str(), 600, 600, 1, data, 600);

   delete[] data;

   return test_map_img;
}

TEST_CASE("IR Sensor Test",
          "[IR][IR_Distance][Sensor-gen]")
{
   string test_map_img = generate_image();

   ObstMap map(test_map_img.c_str());
   BeamModel beam(0.0, 0.0, 300.0, 0.0, 0.0, 0UL);

   IRSensor ir(&map,
               &beam,
               300.0,
               degreesToRadians(50.0));

   //Edge Case 1
   CHECK(ir.generate(300, 300, 2.356) == Approx(300.0).epsilon(0.001));

   //Quadrant 1
   CHECK(ir.generate(300, 300, 0.56) == Approx(93.49331526).epsilon(0.001));
   CHECK(ir.generate(300, 300, 0.785) == Approx(93.49331526).epsilon(0.001));
   CHECK(ir.generate(300, 300, 0.465) == Approx(93.49331526).epsilon(0.001));

   //Quadrant 2
   CHECK(ir.generate(300, 300, 2.39) == Approx(219.31712199).epsilon(0.001));
   CHECK(ir.generate(300, 300, 1.95) == Approx(269.25824035).epsilon(0.001));

   //Quadrant 3
   CHECK(ir.generate(300, 300, 3.59) == Approx(277.30849247).epsilon(0.001));

   //Quadrant 4 (or thereabouts)
   CHECK(ir.generate(300, 300, 4.82) == Approx(135.83077707).epsilon(0.001));
   CHECK(ir.generate(300, 300, 4.66) == Approx(135.83077707).epsilon(0.001)); //Quadrant 3
   CHECK(ir.generate(300, 300, 6.215) == Approx(250.57733337).epsilon(0.001));

   remove(test_map_img.c_str());
}

TEST_CASE("Ultrasound Sensor Test",
          "[Ultrasound][Ultrasound_Distance][Sensor-gen]")
{
   string test_map_img = generate_image();

   ObstMap map(test_map_img.c_str());
   BeamModel beam(0.0, 0.0, 300.0, 0.0, 0.0, 0UL);

   UltrasoundSensor ultrasound(&map,
                               &beam,
                               300.0,
                               degreesToRadians(15.0));

   //Edge Case 1
   CHECK(ultrasound.generate(300, 300, 2.356) == Approx(300.0).epsilon(0.001));

   //Edge Case 2
   CHECK(ultrasound.generate(300, 300, 0.785) == Approx(300.0).epsilon(0.001));

   //Quadrant 1
   CHECK(ultrasound.generate(300, 300, 0.56) == Approx(93.49331526).epsilon(0.001));
   CHECK(ultrasound.generate(300, 300, 0.465) == Approx(93.49331526).epsilon(0.001));

   //Quadrant 2
   CHECK(ultrasound.generate(300, 300, 2.39) == Approx(219.31712199).epsilon(0.001));
   CHECK(ultrasound.generate(300, 300, 1.95) == Approx(269.25824035).epsilon(0.001));

   //Quadrant 3
   CHECK(ultrasound.generate(300, 300, 3.59) == Approx(277.30849247).epsilon(0.001));
   CHECK(ultrasound.generate(300, 300, 4.66) == Approx(200.24984395).epsilon(0.001));

   //Quadrant 4
   CHECK(ultrasound.generate(300, 300, 4.82) == Approx(135.83077707).epsilon(0.001));
   CHECK(ultrasound.generate(300, 300, 6.215) == Approx(250.57733337).epsilon(0.001));

   //No pixel set here
   CHECK(ultrasound.generate(300, 300, 5.00) == 300.0);
   CHECK(ultrasound.generate(300, 300, 1.00) == 300.0);

   remove(test_map_img.c_str());
}

TEST_CASE("Lidar Sensor Test",
          "[Lidar][Lidar_Distance][Sensor-gen]")
{
   string test_map_img = generate_image();

   ObstMap map(test_map_img.c_str());
   BeamModel beam(0.0, 0.0, 300.0, 0.0, 0.0, 0UL);

   Length readings[720];

   Lidar lidar(&map, &beam, 300.0);

   lidar.generate(readings, 300, 300);

   for (int angle = 0; angle < 720; angle++)
   {
      switch (angle)
      {
      case 2 * ((int)radiansToDegrees(0.56)) + 1:
         CHECK(readings[angle] == Approx(93.49331526).epsilon(0.001));
         break;

      case 2 * ((int)radiansToDegrees(0.465)) + 1:
         CHECK(readings[angle] == Approx(223.60679774).epsilon(0.001));
         break;

      case 2 * ((int)radiansToDegrees(4.66)) + 2:
         CHECK(readings[angle] == Approx(200.24984395).epsilon(0.001));
         break;

      case 2 * ((int)radiansToDegrees(4.82)) + 1:
         CHECK(readings[angle] == Approx(135.83077707).epsilon(0.001));
         break;

      case 2 * ((int)radiansToDegrees(6.215)):
         CHECK(readings[angle] == Approx(250.57733337).epsilon(0.001));
         break;

      default:
         CHECK(readings[angle] == Approx(300.0).epsilon(0.001));
         break;
      }
   }

   remove(test_map_img.c_str());
}
