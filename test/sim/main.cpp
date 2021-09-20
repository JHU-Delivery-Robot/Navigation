#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <string>

TEST_CASE("UnfreedMemory")
{
    std::string *unfreed = new std::string("Allocated on heap, not freed");
}
