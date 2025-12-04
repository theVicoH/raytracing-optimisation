#include "test_fixture.hpp"
#include <iostream>
#include <cstdlib>
#include <filesystem>

TestFixture::TestFixture()
{
    char* envRoot = std::getenv("PROJECT_ROOT");
    if (envRoot != nullptr)
    {
        projectRoot = std::string(envRoot);
    }
    else
    {
        auto currentPath = std::filesystem::current_path();

        while (!currentPath.empty() && currentPath != currentPath.root_path())
        {
            if (std::filesystem::exists(currentPath / "CMakeLists.txt") &&
                std::filesystem::exists(currentPath / "scenes"))
            {
                projectRoot = currentPath.string();
                break;
            }
            currentPath = currentPath.parent_path();
        }

        if (projectRoot.empty())
        {
            projectRoot = "/app";
        }
    }

    fixturesPath = projectRoot + "/tests/fixtures";
    referencePath = fixturesPath + "/reference_images";
    outputPath = projectRoot + "/build/test_output";

    std::filesystem::create_directories(outputPath);
    std::filesystem::create_directories(referencePath);
}

bool TestFixture::runRenderTest(
    const std::string& scenePath,
    const std::string& referenceImageName,
    const std::string& testName,
    double psnrThreshold
)
{
    std::cout << "Running test: " << testName << std::endl;
    std::cout << "Project root: " << projectRoot << std::endl;
    std::cout << "Scene path: " << scenePath << std::endl;
    std::cout << "File exists: " << (std::filesystem::exists(scenePath) ? "YES" : "NO") << std::endl;

    auto [scene, camera, image] = SceneLoader::Load(scenePath);

    if (!scene || !camera || !image)
    {
        std::cerr << "Failed to load scene: " << scenePath << std::endl;
        return false;
    }

    std::string outputFile = getOutputPath(testName + ".png");

    PerformanceMetrics metrics;
    metrics.start();

    camera->render(*image, *scene);

    metrics.stop();

    image->writeFile(outputFile);

    PerformanceMetrics::printMetrics(
        testName,
        metrics.getElapsedSeconds(),
        image->width,
        image->height
    );

    std::string referenceFile = getReferencePath(referenceImageName);
    ImageComparisonResult result = ImageComparison::compare(
        referenceFile,
        outputFile,
        psnrThreshold
    );

    PerformanceMetrics::printTestResult(testName, result.passed, result.message);

    if (!result.passed && result.psnr > 0)
    {
        std::string diffFile = getOutputPath(testName + "_diff.png");
        std::cout << "  Saving diff image to: " << diffFile << std::endl;
    }

    delete scene;
    delete camera;
    delete image;

    return result.passed;
}

bool TestFixture::runEdgeCaseTest(
    Scene* scene,
    Camera* camera,
    Image* image,
    const std::string& testName,
    bool shouldSucceed
)
{
    std::cout << "Running edge case test: " << testName << std::endl;

    PerformanceMetrics metrics;
    metrics.start();

    bool renderSucceeded = true;
    try
    {
        camera->render(*image, *scene);
    }
    catch (const std::exception& e)
    {
        renderSucceeded = false;
        std::cout << "  Render threw exception: " << e.what() << std::endl;
    }
    catch (...)
    {
        renderSucceeded = false;
        std::cout << "  Render threw unknown exception" << std::endl;
    }

    metrics.stop();

    bool testPassed = (renderSucceeded == shouldSucceed);

    std::string message;
    if (shouldSucceed)
    {
        message = renderSucceeded ? "Render completed successfully" : "Render failed unexpectedly";
    }
    else
    {
        message = !renderSucceeded ? "Render failed as expected" : "Render succeeded unexpectedly";
    }

    PerformanceMetrics::printTestResult(testName, testPassed, message);
    PerformanceMetrics::printMetrics(
        testName,
        metrics.getElapsedSeconds(),
        image->width,
        image->height
    );

    return testPassed;
}

std::string TestFixture::getScenePath(const std::string& sceneFile) const
{
    return projectRoot + "/scenes/" + sceneFile;
}

std::string TestFixture::getReferencePath(const std::string& imageFile) const
{
    return referencePath + "/" + imageFile;
}

std::string TestFixture::getOutputPath(const std::string& imageFile) const
{
    return outputPath + "/" + imageFile;
}

int TestFixture::exitWithResult(bool passed, const std::string& message)
{
    if (passed)
    {
        std::cout << std::endl << "TEST PASSED: " << message << std::endl;
        return 0;
    }
    else
    {
        std::cerr << std::endl << "TEST FAILED: " << message << std::endl;
        return 1;
    }
}
