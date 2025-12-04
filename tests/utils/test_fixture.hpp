#pragma once
#include <string>
#include "Scene.hpp"
#include "Camera.hpp"
#include "Image.hpp"
#include "SceneLoader.hpp"
#include "image_comparison.hpp"
#include "performance_metrics.hpp"

class TestFixture
{
protected:
    std::string projectRoot;
    std::string fixturesPath;
    std::string referencePath;
    std::string outputPath;

public:
    TestFixture();
    virtual ~TestFixture() = default;

    bool runRenderTest(
        const std::string& scenePath,
        const std::string& referenceImageName,
        const std::string& testName,
        double psnrThreshold = 40.0
    );

    bool runEdgeCaseTest(
        Scene* scene,
        Camera* camera,
        Image* image,
        const std::string& testName,
        bool shouldSucceed = true
    );

    std::string getScenePath(const std::string& sceneFile) const;
    std::string getReferencePath(const std::string& imageFile) const;
    std::string getOutputPath(const std::string& imageFile) const;

    static int exitWithResult(bool passed, const std::string& message);
};
