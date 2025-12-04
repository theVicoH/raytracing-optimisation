#pragma once
#include <string>
#include <vector>
#include <cmath>

struct ImageComparisonResult
{
    bool passed;
    double psnr;
    double mse;
    int diffPixelCount;
    std::string message;
};

class ImageComparison
{
public:
    static ImageComparisonResult compare(
        const std::string& referenceImagePath,
        const std::string& testImagePath,
        double psnrThreshold = 40.0
    );

    static ImageComparisonResult comparePixelData(
        const std::vector<unsigned char>& reference,
        const std::vector<unsigned char>& test,
        unsigned int width,
        unsigned int height,
        double psnrThreshold = 40.0
    );

    static double calculatePSNR(double mse, double maxValue = 255.0);
    static double calculateMSE(
        const std::vector<unsigned char>& img1,
        const std::vector<unsigned char>& img2
    );

    static void saveDiffImage(
        const std::string& outputPath,
        const std::vector<unsigned char>& reference,
        const std::vector<unsigned char>& test,
        unsigned int width,
        unsigned int height
    );
};
