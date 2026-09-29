#include <algorithm>
#include <limits>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "scene.h"
#include "sphere.h"
#include "triangle.h"

TEST_CASE("Mixed scenes select the closest shape in either insertion order", "[scene]") {
    Scene scene;
    scene.objects.push_back(std::make_shared<sphere>(point3(0, 0, -3), 1));

    double triangle_z = -1;
    double expected_t = 1;
    SECTION("Triangle hides sphere") {}
    SECTION("Sphere hides triangle") {
        triangle_z = -5;
        expected_t = 2;
    }

    scene.objects.push_back(std::make_shared<triangle>(
        point3(-1, -1, triangle_z), point3(1, -1, triangle_z),
        point3(0, 1, triangle_z)));

    for (int order = 0; order < 2; ++order) {
        HitStruct hit;
        double tmax = std::numeric_limits<double>::infinity();
        REQUIRE(scene.intersect(ray(point3(0, 0, 0), vec3(0, 0, -1)), 0.001, tmax, hit));
        REQUIRE_THAT(hit.t(), Catch::Matchers::WithinAbs(expected_t, 1e-12));
        REQUIRE_THAT(hit.p().z(), Catch::Matchers::WithinAbs(-expected_t, 1e-12));
        REQUIRE(tmax == hit.t());
        std::reverse(scene.objects.begin(), scene.objects.end());
    }
}

TEST_CASE("Scene misses preserve the output record and search bound", "[scene]") {
    Scene scene;
    double tmin = 0.001;
    double tmax = 10;
    ray r(point3(0, 0, 0), vec3(0, 0, -1));

    SECTION("Empty scene") {}
    SECTION("Mixed scene") {
        scene.objects.push_back(std::make_shared<sphere>(point3(0, 0, -3), 1));
        scene.objects.push_back(std::make_shared<triangle>(
            point3(-1, -1, -1), point3(1, -1, -1), point3(0, 1, -1)));
        SECTION("Ray misses every object") { r = ray(point3(5, 0, 0), vec3(0, 0, -1)); }
        SECTION("Objects are beyond tmax") { tmax = 0.5; }
        SECTION("Objects are before tmin") { tmin = 6; }
    }

    HitStruct hit;
    hit.set_t(42);
    hit.set_p(point3(1, 2, 3));
    hit.set_face_normal(r, vec3(0, 0, 1));
    const double original_tmax = tmax;
    REQUIRE_FALSE(scene.intersect(r, tmin, tmax, hit));
    REQUIRE(tmax == original_tmax);
    REQUIRE(hit.t() == 42);
    REQUIRE(hit.p().x() == 1);
    REQUIRE(hit.p().y() == 2);
    REQUIRE(hit.p().z() == 3);
    REQUIRE(hit.normal().z() == 1);
}

TEST_CASE("Triangle fills a common hit record and rejects invalid intersections", "[scene]") {
    triangle tri(point3(-1, -1, -1), point3(1, -1, -1), point3(0, 1, -1));
    HitStruct hit;
    double tmax = 10;
    REQUIRE(tri.intersect(ray(point3(0, 0, 0), vec3(0, 0, -1)), 0.001, tmax, hit));
    REQUIRE(hit.t() == 1);
    REQUIRE(hit.normal().z() == 1);
    REQUIRE(hit.front_face());

    // A back-face hit is still valid, with its normal facing the incoming ray.
    REQUIRE(tri.intersect(ray(point3(0, 0, -2), vec3(0, 0, 1)), 0.001, tmax, hit));
    REQUIRE_FALSE(hit.front_face());
    REQUIRE(hit.normal().z() == -1);

    REQUIRE_FALSE(tri.intersect(ray(point3(2, 0, 0), vec3(0, 0, -1)), 0.001, tmax, hit));
    REQUIRE_FALSE(tri.intersect(ray(point3(0, 0, 0), vec3(1, 0, 0)), 0.001, tmax, hit));
    triangle degenerate(point3(0, 0, -1), point3(1, 0, -1), point3(2, 0, -1));
    REQUIRE_FALSE(degenerate.intersect(ray(point3(0, 0, 0), vec3(0, 0, -1)), 0.001, tmax, hit));
}
