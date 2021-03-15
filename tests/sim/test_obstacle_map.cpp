#include <string>
#include "catch/catch.hpp"
#include "obstacle_map.hpp"
#include "stb/stb_image_write.h"

#define COORD_TO_IN(c, r, n_c) (r * n_c + c)

using std::string;

/**
 * Test version of the obstacle map class
 *
 * Protected fields have been broken out to facilitate testing without having to
 * integrate the test class into the original API
 *
 * additional access methids should be added for all protected members that
 * require testing.
 */
class ObstMapUUT : public ObstMap {
public:
    /**
     * Constructor to pass arg on
     */
    ObstMapUUT(std::string pathname) :
        ObstMap(pathname) {}

    /**
     * access method for raw map
     */
    std::vector<uint8_t>& getMap() {
        return map;
    }
    int getWidth() {
        return width;
    }
    int getHeight() {
        return height;
    }
};

/**
 * Verify that a map matches an input image
 * @param map obstable map to check
 * @param img_data image to compare against
 */
bool verify_match(ObstMapUUT map, char *img_data) {
    unsigned char *img = reinterpret_cast<unsigned char*>(img_data);
    std::vector<uint8_t> map_vals = map.getMap();
    size_t img_size = map.getHeight()*map.getWidth() * 3;
    for(size_t i = 0; i <= img_size - 3; i += 3) {
        if(img[i] == 255 && img[i+1] == 255 && img[i+2] == 255
           && map_vals[i/3] != 1) {
            return false;
        }
        if(img[i] != 255 && img[i+1] != 255 && img[i+2] != 255
                  && map_vals[i/3] == 1) {
            return false;
        }
    }
    return true;
}

/**
 * Verify that a map matches a flat image
 * @param map obstable map to check
 * @param img_data image to compare against
 */
bool verify_match_flat(ObstMapUUT map, char *img_data) {
    unsigned char *img = reinterpret_cast<unsigned char*>(img_data);
    std::vector<uint8_t> map_vals = map.getMap();
    size_t img_size = map.getHeight()*map.getWidth();
    for(size_t i = 0; i < img_size; i += 1) {
        if(img[i] == 255 && map_vals[i] != 1) {
            return false;
        }
        if(img[i] != 255 && map_vals[i] == 1) {
            return false;
        }
    }
    return true;
}

/**
 * Verify that a map matches an input image
 * @param map obstable map to check
 * @param img_data image to compare against
 */
string generate_image() {
    string test_map_img = "omap_distance_test.png";
    size_t data_size = 600*600;
    char *data = new char[data_size];

    for (size_t i = 0; i < data_size; ++i) {
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

TEST_CASE("Obstacle Map Distance Testing Unlimited",
          "[ObstMap][ObstMap_Distance]") {
    // some setup code
    string test_map_img = generate_image();

    ObstMapUUT uut(test_map_img.c_str());
    REQUIRE(uut.getHeight() == 600);
    REQUIRE(uut.getWidth() == 600);

    //Edge Case 1
    CHECK(uut.distToObstacle(300, 300, 2.356) == Approx(424.26406871).epsilon(0.001));

    //Edge Case 2
    CHECK(uut.distToObstacle(300, 300, 0.785) == Approx(422.849855).epsilon(0.001));

    //Quadrant 1
    CHECK(uut.distToObstacle(300, 300, 0.56) == Approx(93.49331526).epsilon(0.001));
    CHECK(uut.distToObstacle(300, 300, 0.465) == Approx(223.60679774).epsilon(0.001));
    
    //Quadrant 2
    CHECK(uut.distToObstacle(300, 300, 2.39) == Approx(219.31712199).epsilon(0.001));
    CHECK(uut.distToObstacle(300, 300, 1.95) == Approx(269.25824035).epsilon(0.001));
    
    //Quadrant 3
    CHECK(uut.distToObstacle(300, 300, 3.59) == Approx(277.30849247).epsilon(0.001));
    CHECK(uut.distToObstacle(300, 300, 4.66) == Approx(200.24984395).epsilon(0.001));
    
    //Quadrant 4
    CHECK(uut.distToObstacle(300, 300, 4.82) == Approx(135.83077707).epsilon(0.001));
    CHECK(uut.distToObstacle(300, 300, 6.215) == Approx(250.57733337).epsilon(0.001));

    //No pixel set here
    CHECK(uut.distToObstacle(300, 300, 5.00) == -4.0);
    CHECK(uut.distToObstacle(300, 300, 1.00) == -4.0);
    
    remove(test_map_img.c_str());
}

TEST_CASE("Obstacle Map Distance Testing Limited",
          "[ObstMap][ObstMap_Distance]") {
    // some setup code
    string test_map_img = generate_image();

    ObstMapUUT uut(test_map_img.c_str());
    REQUIRE(uut.getHeight() == 600);
    REQUIRE(uut.getWidth() == 600);

    //Edge Case 1
    CHECK(uut.distToObstacleLimited(300, 300, 2.356, 424) == 424.0);

    //Edge Case 2
    CHECK(uut.distToObstacleLimited(300, 300, 0.785, 423) == Approx(422.849855).epsilon(0.001));

    //Quadrant 1
    CHECK(uut.distToObstacleLimited(300, 300, 0.56, 93) == 93.0);
    CHECK(uut.distToObstacleLimited(300, 300, 0.465, 224) == Approx(223.60679774).epsilon(0.001));
    
    //Quadrant 2
    CHECK(uut.distToObstacleLimited(300, 300, 2.39, 219) == 219.0);
    CHECK(uut.distToObstacleLimited(300, 300, 1.95, 270) == Approx(269.25824035).epsilon(0.001));
    
    //Quadrant 3
    CHECK(uut.distToObstacleLimited(300, 300, 3.59, 277) == 277.0);
    CHECK(uut.distToObstacleLimited(300, 300, 4.66, 201) == Approx(200.24984395).epsilon(0.001));
    
    //Quadrant 4
    CHECK(uut.distToObstacleLimited(300, 300, 4.82, 135) == 135.0);
    CHECK(uut.distToObstacleLimited(300, 300, 6.215, 251) == Approx(250.57733337).epsilon(0.001));
    
    remove(test_map_img.c_str());
}

TEST_CASE("Obstacle Map File Loading - 1000x100 all white",
          "[ObstMap][ObstMap_Loading]") {
    // some setup code
    const char *test_map_img = "omap_loading_test.png";
    size_t data_size = 1000*100*3;
    char *data = new char[data_size];
    for(size_t i = 0; i < data_size; ++i) {
        data[i] = static_cast<char>(255);
    }
    stbi_write_png(test_map_img, 1000, 100, 3, data, 100*3);

    ObstMapUUT uut(test_map_img);
    REQUIRE(uut.getHeight() == 100);
    REQUIRE(uut.getWidth() == 1000);
    REQUIRE(verify_match(uut, data));

    delete[] data;
    remove(test_map_img);
}

TEST_CASE("Obstacle Map File Loading - 1000x100 all black",
          "[ObstMap][ObstMap_Loading]") {
    // some setup code
    const char *test_map_img = "omap_loading_test.png";
    size_t data_size = 1000*100*3;
    char *data = new char[data_size];
    for(size_t i = 0; i < data_size; ++i) {
        data[i] = static_cast<char>(0);
    }
    stbi_write_png(test_map_img, 1000, 100, 3, data, 100*3);

    ObstMapUUT uut(test_map_img);
    REQUIRE(uut.getHeight() == 100);
    REQUIRE(uut.getWidth() == 1000);
    REQUIRE(verify_match(uut, data));

    delete[] data;
    remove(test_map_img);
}

TEST_CASE("Obstacle Map File Loading - 100x1000 checkerboard",
          "[ObstMap][ObstMap_Loading]") {
    // some setup code
    const char *test_map_img = "omap_loading_test.png";
    size_t data_size = 100*1000*3;
    char *data = new char[data_size];
    // generate checkerboard
    for(size_t r = 0; r < 1000; ++r) {
        for(size_t c = 0; c < 100; ++c) {
            // 25x100 checkbaord suqares, starting white and then alternating
            if(((c/25) % 2 == 1 && (r/100) % 2 == 1)
               || ((c/25) % 2 != 1 && (r/100) % 2 != 1)) {
                data[(r*100*3) + c*3] = 0;
                data[(r*100*3) + c*3 + 1] = 0;
                data[(r*100*3) + c*3 + 2] = 0;
            } else {
                data[(r*100*3) + c*3] = static_cast<char>(255);
                data[(r*100*3) + c*3 + 1] = static_cast<char>(255);
                data[(r*100*3) + c*3 + 2] = static_cast<char>(255);
            }
        }
    }
    stbi_write_png(test_map_img, 100, 1000, 3, data, 100*3);

    ObstMapUUT uut(test_map_img);
    REQUIRE(uut.getHeight() == 1000);
    REQUIRE(uut.getWidth() == 100);
    REQUIRE(verify_match(uut, data));

    delete[] data;
    remove(test_map_img);
}

TEST_CASE("Obstacle Map File Loading Flat - 100x1000 checkerboard",
          "[ObstMap][ObstMap_Loading]") {
    // some setup code
    const char *test_map_img = "omap_loading_test.png";
    size_t data_size = 100*1000;
    char *data = new char[data_size];
    // generate checkerboard
    for(size_t r = 0; r < 1000; ++r) {
        for(size_t c = 0; c < 100; ++c) {
            // 25x100 checkbaord suqares, starting white and then alternating
            if(((c/25) % 2 == 1 && (r/100) % 2 == 1)
               || ((c/25) % 2 != 1 && (r/100) % 2 != 1)) {
                data[(r*100) + c] = 0;
            } else {
                data[(r*100) + c] = static_cast<char>(255);
            }
        }
    }
    stbi_write_png(test_map_img, 100, 1000, 1, data, 100*1);

    ObstMapUUT uut(test_map_img);
    REQUIRE(uut.getHeight() == 1000);
    REQUIRE(uut.getWidth() == 100);
    REQUIRE(verify_match_flat(uut, data));

    delete[] data;
    remove(test_map_img);
}