#include "catch2/catch.hpp"

#include "common/common.hpp"
#include "common/vector2.hpp"

using namespace common;

TEST_CASE("Vector2 Polar", "[Vector2]")
{
    Vector2 x = Vector2::polar(0.0, 5.0);
    Vector2 neg_x1 = Vector2::polar(0.0, -5.0);
    Vector2 neg_x2 = Vector2::polar(PI, 5.0);
    Vector2 y = Vector2::polar(0.5 * PI, -5.0);

    CHECK(x.x == Approx(5.0).margin(1e-6));
    CHECK(x.y == Approx(0.0).margin(1e-6));

    CHECK(neg_x1.x == Approx(-5.0).margin(1e-6));
    CHECK(neg_x1.y == Approx(0.0).margin(1e-6));

    CHECK(neg_x2.x == Approx(-5.0).margin(1e-6));
    CHECK(neg_x2.y == Approx(0.0).margin(1e-6));

    CHECK(y.x == Approx(0.0).margin(1e-6));
    CHECK(y.y == Approx(-5.0).margin(1e-6));
}

TEST_CASE("Vector2 Magnitude", "[Vector2]")
{
    Vector2 zero = Vector2::polar(0.0, 0.0);
    Vector2 one = Vector2(0.0, -1.0);
    Vector2 two = Vector2::polar(1.5 * PI, -2.0);
    Vector2 five = Vector2(-3.0, 4.0);
    Vector2 sixEightyNine = Vector2(-364, 585);

    CHECK(zero.magnitude() == Approx(0.0).margin(1e-6));
    CHECK(one.magnitude() == Approx(1.0).margin(1e-6));
    CHECK(two.magnitude() == Approx(2.0).margin(1e-6));
    CHECK(five.magnitude() == Approx(5.0).margin(1e-6));
    CHECK(sixEightyNine.magnitude() == Approx(689.0).margin(1e-6));
}

TEST_CASE("Vector2 Dot Product", "[Vector2]")
{
    Vector2 zero = Vector2::polar(0.0, 0.0);
    Vector2 one = Vector2(0.0, -1.0);
    Vector2 two = Vector2::polar(1.5 * PI, -2.0);
    Vector2 five = Vector2(-3.0, 4.0);
    Vector2 sixEightyNine = Vector2(-364, 585);

    SECTION("Dot product with zero vector is always zero")
    {
        CHECK(zero.dot(one) == Approx(0.0).margin(1e-6));
        CHECK(zero.dot(two) == Approx(0.0).margin(1e-6));
        CHECK(zero.dot(five) == Approx(0.0).margin(1e-6));
        CHECK(zero.dot(sixEightyNine) == Approx(0.0).margin(1e-6));

        CHECK(one.dot(zero) == Approx(0.0).margin(1e-6));
        CHECK(two.dot(zero) == Approx(0.0).margin(1e-6));
        CHECK(five.dot(zero) == Approx(0.0).margin(1e-6));
        CHECK(sixEightyNine.dot(zero) == Approx(0.0).margin(1e-6));
    }

    SECTION("Dot product with self gives square of magnitude")
    {
        CHECK(zero.dot(zero) == Approx(0.0).margin(1e-6));
        CHECK(one.dot(one) == Approx(1.0).margin(1e-6));
        CHECK(two.dot(two) == Approx(4.0).margin(1e-6));
        CHECK(five.dot(five) == Approx(25.0).margin(1e-6));
        CHECK(sixEightyNine.dot(sixEightyNine) == Approx(689.0 * 689.0).margin(1e-6));
    }

    SECTION("Dot product of orthogonal vectors is zero")
    {
        Vector2 a(-5.0, 5.0);
        Vector2 b(-2.0, -2.0);

        CHECK(a.dot(b) == Approx(0.0).margin(1e-6));
        CHECK(b.dot(a) == Approx(0.0).margin(1e-6));
    }

    SECTION("Dot product of parallel vectors is +/-1")
    {
        Vector2 a(-5.0, 5.0);
        Vector2 b(2.0, -2.0);
        Vector2 c(3.0, -3.0);

        CHECK(a.dot(b) == Approx(-20.0).margin(1e-6));
        CHECK(b.dot(c) == Approx(12.0).margin(1e-6));
    }
}

TEST_CASE("Vector2 Addition", "[Vector2]")
{
    Vector2 zero = Vector2::polar(0.0, 0.0);
    Vector2 a = Vector2(0.0, -1.0);
    Vector2 b = Vector2(12.0, -1.0);
    Vector2 c = Vector2(3.0, 16.0);

    CHECK((zero + b).x == Approx(12.0).margin(1e-6));
    CHECK((zero + b).y == Approx(-1.0).margin(1e-6));

    a += c;
    CHECK(a.x == Approx(3.0).margin(1e-6));
    CHECK(a.y == Approx(15.0).margin(1e-6));

    a -= b;
    CHECK(a.x == Approx(-9.0).margin(1e-6));
    CHECK(a.y == Approx(16.0).margin(1e-6));

    CHECK((b + c).x == Approx(15.0).margin(1e-6));
    CHECK((b + c).y == Approx(15.0).margin(1e-6));

    CHECK((b - c).x == Approx(9.0).margin(1e-6));
    CHECK((b - c).y == Approx(-17.0).margin(1e-6));
}

TEST_CASE("Vector2 Scalar Multiplication", "[Vector2]")
{
    Vector2 zero = Vector2::polar(0.0, 0.0);
    Vector2 a = Vector2(5.0, -1.0);
    Vector2 b = Vector2(12.0, -1.0);

    CHECK((2 * zero).x == Approx(0.0).margin(1e-6));
    CHECK((2 * zero).y == Approx(0.0).margin(1e-6));

    CHECK((0 * a).x == Approx(0.0).margin(1e-6));
    CHECK((0 * a).y == Approx(0.0).margin(1e-6));

    CHECK((-1 * b).x == Approx(-12.0).margin(1e-6));
    CHECK((-1 * b).y == Approx(1.0).margin(1e-6));

    CHECK((b * -1).x == Approx(-12.0).margin(1e-6));
    CHECK((b * -1).y == Approx(1.0).margin(1e-6));
}
