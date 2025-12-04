#include "test_fixture.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    TestFixture fixture;

    std::string scenePath = fixture.getScenePath("sphere-galaxy-on-plane.json");
    std::string referenceName = "sphere-galaxy-on-plane.png";

    bool passed = fixture.runRenderTest(
        scenePath,
        referenceName,
        "MultiObjectGalaxy",
        35.0
    );

    return TestFixture::exitWithResult(passed, "Multi-object scene rendering test");
}
