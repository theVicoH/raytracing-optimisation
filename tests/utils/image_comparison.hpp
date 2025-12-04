#pragma once
#include <string>
#include <vector>
#include <cmath>

struct ImageComparisonResult
{
    bool passed;
    std::string hash1;
    std::string hash2;
    std::string message;
};

class ImageComparison
{
public:
    static ImageComparisonResult compare(
        const std::string& referenceImagePath,
        const std::string& testImagePath,
        double psnrThreshold = 40.0  // Kept for compatibility but unused
    );

    static std::string calculateHash(const std::vector<unsigned char>& data);
    static std::string calculateFileHash(const std::string& filepath);
};
