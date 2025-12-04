#include "test_fixture.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    TestFixture fixture;

    std::string scenePath = fixture.getScenePath("two-spheres-on-plane.json");
    std::string referenceName = "two-spheres-on-plane.png";

    bool passed = fixture.runRenderTest(
        scenePath,
        referenceName,
        "SimpleSphereTwoOnPlane",
        35.0
    );

    return TestFixture::exitWithResult(passed, "Simple sphere rendering test");
}
