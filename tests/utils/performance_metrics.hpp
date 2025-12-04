#pragma once
#include <chrono>
#include <string>

class PerformanceMetrics
{
private:
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point endTime;
    bool isRunning;

public:
    PerformanceMetrics();

    void start();
    void stop();
    double getElapsedSeconds() const;
    double getElapsedMilliseconds() const;

    static void printMetrics(
        const std::string& testName,
        double renderTimeSeconds,
        int imageWidth,
        int imageHeight
    );

    static void printTestResult(
        const std::string& testName,
        bool passed,
        const std::string& message
    );
};
