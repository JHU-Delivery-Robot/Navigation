#include "catch2/catch.hpp"

#include "common/common.hpp"
#include "common/coordinate_system.hpp"

using namespace common;

TEST_CASE("CoordinateSystem Gilman Hall", "[CoordinateSystem]")
{
    common::Coordinates gilman_hall(39.3289, -76.6215);
    common::CoordinateSystem cs(gilman_hall);

    common::Coordinates gilman_tunnel_south(39.32858771926828, -76.62120186208475);
    common::Coordinates gilman_tunnel_north(39.32938856944643, -76.62126690565151);
    double gilman_tunnel_length = 89.2;

    common::Vector2 gilman_tunnel = cs.project(gilman_tunnel_north) - cs.project(gilman_tunnel_south);

    CHECK(gilman_tunnel.magnitude() == Approx(gilman_tunnel_length).epsilon(1e-3));

    common::Vector2 gilman_south_vec = cs.project(gilman_tunnel_south);
    common::Coordinates gilman_north_estimated = cs.project(gilman_south_vec + gilman_tunnel);

    CHECK(gilman_north_estimated.latitude == Approx(gilman_tunnel_north.latitude).epsilon(1e-3));
    CHECK(gilman_north_estimated.longitude == Approx(gilman_tunnel_north.longitude).epsilon(1e-3));
}

TEST_CASE("CoordinateSystem Keyser Quad", "[CoordinateSystem]") {
    common::Coordinates gilman_hall(39.3289, -76.6215);
    common::CoordinateSystem cs(gilman_hall);

    common::Coordinates keyser_ne(39.32929418789203, -76.61977226499927);
    common::Coordinates keyser_se(39.328791582186895, -76.61973940794036);
    common::Coordinates keyser_sw(39.328743344296534, -76.6209879762223);
    common::Coordinates keyser_nw(39.3292449129833, -76.62102552714603);

    common::Vector2 keyser_ne_v = cs.project(keyser_ne);
    common::Vector2 keyser_se_v = cs.project(keyser_se);
    common::Vector2 keyser_sw_v = cs.project(keyser_sw);
    common::Vector2 keyser_nw_v = cs.project(keyser_nw);

    double diagonal_ne_to_sw = 121.15;
    double diagonal_nw_to_se = 121.51;
    double ne_to_nw = 107.93;
    double ne_to_se = 55.96;
    double sw_to_se = 107.52;
    double sw_to_nw = 55.81;

    common::Vector2 diagonal_ne_to_sw_v = keyser_sw_v - keyser_ne_v;
    common::Vector2 diagonal_nw_to_se_v = keyser_se_v - keyser_nw_v;
    common::Vector2 ne_to_nw_v = keyser_nw_v - keyser_ne_v;
    common::Vector2 ne_to_se_v = keyser_se_v - keyser_ne_v;
    common::Vector2 sw_to_se_v = keyser_se_v - keyser_sw_v;
    common::Vector2 sw_to_nw_v = keyser_nw_v - keyser_sw_v;

    CHECK(diagonal_ne_to_sw_v.magnitude() == Approx(diagonal_ne_to_sw).epsilon(1e-3));
    CHECK(diagonal_nw_to_se_v.magnitude() == Approx(diagonal_nw_to_se).epsilon(1e-3));
    CHECK(ne_to_nw_v.magnitude() == Approx(ne_to_nw).epsilon(1e-3));
    CHECK(ne_to_se_v.magnitude() == Approx(ne_to_se).epsilon(1e-3));
    CHECK(sw_to_se_v.magnitude() == Approx(sw_to_se).epsilon(1e-3));
    CHECK(sw_to_nw_v.magnitude() == Approx(sw_to_nw).epsilon(1e-3));
}
