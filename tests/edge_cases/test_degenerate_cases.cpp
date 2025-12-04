#include "test_fixture.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{
    std::cout << "Running edge case test: Degenerate cases" << std::endl;
    std::cout << "Testing: Empty scene (no objects) and scene with no lights" << std::endl;
    std::cout << std::endl;

    TestFixture fixture;
    bool allTestsPassed = true;

    std::cout << "Test 1: Empty scene (no objects)" << std::endl;
    {
        std::string testSceneFile = fixture.getScenePath("test_empty.json");
        std::ofstream sceneFile(testSceneFile);
        sceneFile << R"({
    "image": {"width": 320, "height": 240},
    "reflections": 0,
    "ambient": {"r": 0.2, "g": 0.2, "b": 0.3},
    "lights": [],
    "objects": []
})";
        sceneFile.close();

        auto [scene, camera, image] = SceneLoader::Load(testSceneFile);

        bool passed = fixture.runEdgeCaseTest(
            scene,
            camera,
            image,
            "DegenerateEmptyScene",
            true
        );

        std::string outputFile = fixture.getOutputPath("degenerate_empty.png");
        image->writeFile(outputFile);

        delete scene;
        delete camera;
        delete image;

        allTestsPassed = allTestsPassed && passed;
    }

    std::cout << std::endl << "Test 2: Scene with objects but no lights" << std::endl;
    {
        std::string testSceneFile = fixture.getScenePath("test_no_lights.json");
        std::ofstream sceneFile(testSceneFile);
        sceneFile << R"({
    "image": {"width": 320, "height": 240},
    "reflections": 0,
    "ambient": {"r": 0.1, "g": 0.1, "b": 0.1},
    "lights": [],
    "objects": [{
        "type": "sphere",
        "radius": 1.0,
        "position": {"x": 0, "y": 0, "z": 5},
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

        bool passed = fixture.runEdgeCaseTest(
            scene,
            camera,
            image,
            "DegenerateNoLights",
            true
        );

        std::string outputFile = fixture.getOutputPath("degenerate_no_lights.png");
        image->writeFile(outputFile);

        delete scene;
        delete camera;
        delete image;

        allTestsPassed = allTestsPassed && passed;
    }

    return TestFixture::exitWithResult(allTestsPassed, "Degenerate cases edge case tests");
}
