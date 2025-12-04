#include "test_fixture.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{
    std::cout << "Running edge case test: Numerical precision" << std::endl;
    std::cout << "Testing: Very small coordinate differences (floating-point precision)" << std::endl;
    std::cout << std::endl;

    TestFixture fixture;

    std::string testSceneFile = fixture.getScenePath("test_precision.json");

    std::ofstream sceneFile(testSceneFile);
    sceneFile << R"({
    "image": {"width": 320, "height": 240},
    "reflections": 0,
    "ambient": {"r": 1, "g": 1, "b": 1},
    "lights": [{
        "type": "point",
        "position": {"x": 5, "y": 10, "z": -5},
        "diffuse": {"r": 0.5, "g": 0.5, "b": 0.5},
        "specular": {"r": 1, "g": 1, "b": 1}
    }],
    "objects": [
        {
            "type": "sphere",
            "radius": 1.0,
            "position": {"x": 0.000001, "y": 0, "z": 5},
            "material": {
                "type": "phong",
                "ambient": {"r": 1, "g": 0, "b": 0},
                "diffuse": {"r": 1, "g": 1, "b": 1},
                "specular": {"r": 1, "g": 1, "b": 1},
                "shininess": 40
            }
        },
        {
            "type": "sphere",
            "radius": 0.5,
            "position": {"x": 2.0000001, "y": 0, "z": 5},
            "material": {
                "type": "phong",
                "ambient": {"r": 0, "g": 1, "b": 0},
                "diffuse": {"r": 1, "g": 1, "b": 1},
                "specular": {"r": 1, "g": 1, "b": 1},
                "shininess": 40
            }
        }
    ]
})";
    sceneFile.close();

    auto [scene, camera, image] = SceneLoader::Load(testSceneFile);

    bool testPassed = fixture.runEdgeCaseTest(
        scene,
        camera,
        image,
        "NumericalPrecision",
        true
    );

    std::string outputFile = fixture.getOutputPath("numerical_precision.png");
    image->writeFile(outputFile);

    delete scene;
    delete camera;
    delete image;

    return TestFixture::exitWithResult(testPassed, "Numerical precision edge case test");
}
