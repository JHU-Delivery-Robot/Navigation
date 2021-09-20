#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <string>

TEST_CASE("Fail")
{
    std::string *unfreed = new std::string("Allocated on heap, not freed");
    REQUIRE(unfreed == nullptr); // fail on purpose
}
