#include "test_fixture.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    TestFixture fixture;

    std::string scenePath = fixture.getScenePath("monkey-on-plane.json");
    std::string referenceName = "monkey-on-plane.png";

    bool passed = fixture.runRenderTest(
        scenePath,
        referenceName,
        "ComplexGeometryMonkey",
        35.0
    );

    return TestFixture::exitWithResult(passed, "Complex geometry (mesh) rendering test");
}
