#include <filesystem>
#include <type_traits>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "orthographic_camera.h"
#include "perspective_camera.h"

// Observe the rays that the actual render loop sends into the scene.
class RayRecorder : public Shape {
public:
    mutable std::vector<ray> rays;

    bool intersect(const ray& r, double, double&, HitStruct&) const override {
        rays.push_back(r);
        return false;
    }
};

static_assert(std::is_abstract_v<camera>);

TEST_CASE("Camera projections generate the expected rays through the render loop", "[camera]") {
    std::unique_ptr<camera> cam;
    bool perspective = false;
    SECTION("Perspective") {
        cam = std::make_unique<PerspectiveCamera>();
        perspective = true;
    }
    SECTION("Orthographic") {
        cam = std::make_unique<OrthographicCamera>();
    }

    // Looking along -X tests that projection follows the camera's orientation,
    // rather than assuming the original camera looking down -Z.
    cam->lookfrom = point3(2, 1, 3);
    cam->lookat = point3(-1, 1, 3);
    cam->image_width = 3;
    Scene scene;
    auto recorder = std::make_shared<RayRecorder>();
    scene.objects.push_back(recorder);

    const auto output = std::filesystem::temp_directory_path() /
        (perspective ? "raytracing-test-perspective.png" : "raytracing-test-orthographic.png");
    cam->render(scene, output.string());
    REQUIRE(std::filesystem::file_size(output) > 0);
    std::filesystem::remove(output);

    REQUIRE(recorder->rays.size() == 9);
    const ray& center = recorder->rays[4];
    REQUIRE_THAT((center.origin() - cam->lookfrom).length(), Catch::Matchers::WithinAbs(0, 1e-12));
    REQUIRE_THAT((center.direction() - vec3(-1, 0, 0)).length(), Catch::Matchers::WithinAbs(0, 1e-12));

    for (const ray& r : recorder->rays) {
        if (perspective) {
            REQUIRE_THAT((r.origin() - cam->lookfrom).length(), Catch::Matchers::WithinAbs(0, 1e-12));
        } else {
            REQUIRE_THAT((r.direction() - center.direction()).length(), Catch::Matchers::WithinAbs(0, 1e-12));
            // All origins lie on the camera plane, perpendicular to -X.
            REQUIRE_THAT(r.origin().x(), Catch::Matchers::WithinAbs(2, 1e-12));
        }
    }

    // Top pixels point/start above the center; right pixels lie toward -Z
    // for this camera pose. This also verifies image row orientation.
    const ray& top = recorder->rays[1];
    const ray& right = recorder->rays[5];
    if (perspective) {
        REQUIRE(top.direction().y() > center.direction().y());
        REQUIRE(right.direction().z() < center.direction().z());
    } else {
        REQUIRE(top.origin().y() > center.origin().y());
        REQUIRE(right.origin().z() < center.origin().z());
    }
}
