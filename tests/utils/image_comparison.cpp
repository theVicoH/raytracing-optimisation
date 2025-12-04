#include "image_comparison.hpp"
#include "lodepng.h"
#include <iostream>
#include <cmath>
#include <algorithm>

ImageComparisonResult ImageComparison::compare(
    const std::string& referenceImagePath,
    const std::string& testImagePath,
    double psnrThreshold
)
{
    ImageComparisonResult result;
    result.passed = false;
    result.psnr = 0.0;
    result.mse = 0.0;
    result.diffPixelCount = 0;

    std::vector<unsigned char> referenceImage;
    std::vector<unsigned char> testImage;
    unsigned int refWidth, refHeight;
    unsigned int testWidth, testHeight;

    unsigned int errorRef = lodepng::decode(referenceImage, refWidth, refHeight, referenceImagePath);
    if (errorRef)
    {
        result.message = "Failed to load reference image: " + std::string(lodepng_error_text(errorRef));
        return result;
    }

    unsigned int errorTest = lodepng::decode(testImage, testWidth, testHeight, testImagePath);
    if (errorTest)
    {
        result.message = "Failed to load test image: " + std::string(lodepng_error_text(errorTest));
        return result;
    }

    if (refWidth != testWidth || refHeight != testHeight)
    {
        result.message = "Image dimensions mismatch: reference(" +
                        std::to_string(refWidth) + "x" + std::to_string(refHeight) +
                        ") vs test(" + std::to_string(testWidth) + "x" + std::to_string(testHeight) + ")";
        return result;
    }

    return comparePixelData(referenceImage, testImage, refWidth, refHeight, psnrThreshold);
}

ImageComparisonResult ImageComparison::comparePixelData(
    const std::vector<unsigned char>& reference,
    const std::vector<unsigned char>& test,
    unsigned int width,
    unsigned int height,
    double psnrThreshold
)
{
    ImageComparisonResult result;
    result.diffPixelCount = 0;

    if (reference.size() != test.size())
    {
        result.passed = false;
        result.message = "Image data size mismatch";
        return result;
    }

    result.mse = calculateMSE(reference, test);
    result.psnr = calculatePSNR(result.mse);

    const double PIXEL_TOLERANCE = 5.0;
    for (size_t i = 0; i < reference.size(); i += 4)
    {
        double rDiff = std::abs(static_cast<int>(reference[i]) - static_cast<int>(test[i]));
        double gDiff = std::abs(static_cast<int>(reference[i + 1]) - static_cast<int>(test[i + 1]));
        double bDiff = std::abs(static_cast<int>(reference[i + 2]) - static_cast<int>(test[i + 2]));

        if (rDiff > PIXEL_TOLERANCE || gDiff > PIXEL_TOLERANCE || bDiff > PIXEL_TOLERANCE)
        {
            result.diffPixelCount++;
        }
    }

    result.passed = result.psnr >= psnrThreshold;

    if (result.passed)
    {
        result.message = "Images match (PSNR: " + std::to_string(result.psnr) + " dB)";
    }
    else
    {
        result.message = "Images differ (PSNR: " + std::to_string(result.psnr) +
                        " dB, threshold: " + std::to_string(psnrThreshold) +
                        " dB, diff pixels: " + std::to_string(result.diffPixelCount) + ")";
    }

    return result;
}

double ImageComparison::calculateMSE(
    const std::vector<unsigned char>& img1,
    const std::vector<unsigned char>& img2
)
{
    if (img1.size() != img2.size())
    {
        return -1.0;
    }

    double sum = 0.0;
    for (size_t i = 0; i < img1.size(); ++i)
    {
        double diff = static_cast<double>(img1[i]) - static_cast<double>(img2[i]);
        sum += diff * diff;
    }

    return sum / static_cast<double>(img1.size());
}

double ImageComparison::calculatePSNR(double mse, double maxValue)
{
    if (mse == 0.0)
    {
        return 100.0;
    }

    return 10.0 * std::log10((maxValue * maxValue) / mse);
}

void ImageComparison::saveDiffImage(
    const std::string& outputPath,
    const std::vector<unsigned char>& reference,
    const std::vector<unsigned char>& test,
    unsigned int width,
    unsigned int height
)
{
    std::vector<unsigned char> diffImage(reference.size());

    for (size_t i = 0; i < reference.size(); i += 4)
    {
        int rDiff = std::abs(static_cast<int>(reference[i]) - static_cast<int>(test[i]));
        int gDiff = std::abs(static_cast<int>(reference[i + 1]) - static_cast<int>(test[i + 1]));
        int bDiff = std::abs(static_cast<int>(reference[i + 2]) - static_cast<int>(test[i + 2]));

        diffImage[i] = std::min(255, rDiff * 10);
        diffImage[i + 1] = std::min(255, gDiff * 10);
        diffImage[i + 2] = std::min(255, bDiff * 10);
        diffImage[i + 3] = 255;
    }

    lodepng::encode(outputPath, diffImage, width, height);
}
