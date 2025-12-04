#include "performance_metrics.hpp"
#include <iostream>
#include <iomanip>

PerformanceMetrics::PerformanceMetrics() : isRunning(false)
{
}

void PerformanceMetrics::start()
{
    startTime = std::chrono::high_resolution_clock::now();
    isRunning = true;
}

void PerformanceMetrics::stop()
{
    endTime = std::chrono::high_resolution_clock::now();
    isRunning = false;
}

double PerformanceMetrics::getElapsedSeconds() const
{
    if (isRunning)
    {
        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(now - startTime);
        return elapsed.count() * 1e-9;
    }
    else
    {
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
        return elapsed.count() * 1e-9;
    }
}

double PerformanceMetrics::getElapsedMilliseconds() const
{
    return getElapsedSeconds() * 1000.0;
}

void PerformanceMetrics::printMetrics(
    const std::string& testName,
    double renderTimeSeconds,
    int imageWidth,
    int imageHeight
)
{
    long long totalPixels = static_cast<long long>(imageWidth) * static_cast<long long>(imageHeight);
    double pixelsPerSecond = totalPixels / renderTimeSeconds;

    std::cout << std::endl;
    std::cout << "Performance Metrics for: " << testName << std::endl;
    std::cout << "  Resolution: " << imageWidth << "x" << imageHeight
              << " (" << totalPixels << " pixels)" << std::endl;
    std::cout << "  Render time: " << std::fixed << std::setprecision(3)
              << renderTimeSeconds << "s" << std::endl;
    std::cout << "  Pixels/second: " << std::fixed << std::setprecision(0)
              << pixelsPerSecond << std::endl;
}

void PerformanceMetrics::printTestResult(
    const std::string& testName,
    bool passed,
    const std::string& message
)
{
    std::cout << std::endl;
    std::cout << "Test: " << testName << std::endl;
    std::cout << "  Status: " << (passed ? "PASSED" : "FAILED") << std::endl;
    std::cout << "  " << message << std::endl;
}
