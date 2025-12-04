#include "test_fixture.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{
    std::cout << "Running edge case test: Boundary conditions" << std::endl;
    std::cout << "Testing: Very small sphere radius (0.01)" << std::endl;
    std::cout << std::endl;

    TestFixture fixture;

    std::string testSceneFile = fixture.getScenePath("test_boundary.json");

    std::ofstream sceneFile(testSceneFile);
    sceneFile << R"({
    "image": {"width": 320, "height": 240},
    "reflections": 0,
    "ambient": {"r": 1, "g": 1, "b": 1},
    "lights": [{
        "type": "point",
        "position": {"x": 0, "y": 5, "z": 0},
        "diffuse": {"r": 0.5, "g": 0.5, "b": 0.5},
        "specular": {"r": 1, "g": 1, "b": 1}
    }],
    "objects": [{
        "type": "sphere",
        "radius": 0.01,
        "position": {"x": 0, "y": 0, "z": 3},
        "material": {
            "type": "phong",
            "ambient": {"r": 1, "g": 0, "b": 0},
            "diffuse": {"r": 1, "g": 1, "b": 1},
            "specular": {"r": 1, "g": 1, "b": 1},
            "shininess": 40
        }
    }]
})";
    sceneFile.close();

    auto [scene, camera, image] = SceneLoader::Load(testSceneFile);

    bool testPassed = fixture.runEdgeCaseTest(
        scene,
        camera,
        image,
        "BoundaryConditions",
        true
    );

    std::string outputFile = fixture.getOutputPath("boundary_conditions.png");
    image->writeFile(outputFile);

    delete scene;
    delete camera;
    delete image;

    return TestFixture::exitWithResult(testPassed, "Boundary conditions edge case test");
}
