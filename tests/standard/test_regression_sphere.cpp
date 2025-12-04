#include "test_fixture.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    std::cout << "Running regression test: Sphere intersection validation" << std::endl;
    std::cout << "This test uses the same scene as simple sphere test to detect regressions" << std::endl;
    std::cout << std::endl;

    TestFixture fixture;

    std::string scenePath = fixture.getScenePath("two-spheres-on-plane.json");
    std::string referenceName = "two-spheres-on-plane.png";

    bool passed = fixture.runRenderTest(
        scenePath,
        referenceName,
        "RegressionSphereIntersection",
        35.0
    );

    return TestFixture::exitWithResult(passed, "Sphere intersection regression test");
}
