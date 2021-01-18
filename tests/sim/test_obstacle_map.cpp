#include "catch/catch.hpp"
#include "obstacle_map.hpp"
#include "stb/stb_image_write.h"

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
