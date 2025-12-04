#include "image_comparison.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

ImageComparisonResult ImageComparison::compare(
    const std::string& referenceImagePath,
    const std::string& testImagePath,
    double psnrThreshold
)
{
    ImageComparisonResult result;
    result.passed = false;

    std::ifstream refFile(referenceImagePath, std::ios::binary);
    std::ifstream testFile(testImagePath, std::ios::binary);

    if (!refFile.is_open())
    {
        result.message = "Failed to load reference image: " + referenceImagePath;
        return result;
    }

    if (!testFile.is_open())
    {
        result.message = "Failed to load test image: " + testImagePath;
        return result;
    }

    std::vector<unsigned char> refData(std::istreambuf_iterator<char>(refFile), {});
    std::vector<unsigned char> testData(std::istreambuf_iterator<char>(testFile), {});

    refFile.close();
    testFile.close();

    result.hash1 = calculateHash(refData);
    result.hash2 = calculateHash(testData);

    if (refData.size() != testData.size())
    {
        result.passed = false;
        result.message = "Images differ (file size mismatch: " +
                        std::to_string(refData.size()) + " vs " +
                        std::to_string(testData.size()) + " bytes)";
        return result;
    }

    bool identical = (refData == testData);
    result.passed = identical;

    if (result.passed)
    {
        result.message = "Images match (byte-perfect, hash: " + result.hash1.substr(0, 16) + "...)";
    }
    else
    {
        size_t diffCount = 0;
        for (size_t i = 0; i < refData.size(); i++)
        {
            if (refData[i] != testData[i])
            {
                diffCount++;
            }
        }
        result.message = "Images differ (" + std::to_string(diffCount) + " different bytes out of " +
                        std::to_string(refData.size()) + ")";
    }

    return result;
}

std::string ImageComparison::calculateFileHash(const std::string& filepath)
{
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open())
    {
        return "";
    }

    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(file), {});
    file.close();

    return calculateHash(buffer);
}

std::string ImageComparison::calculateHash(const std::vector<unsigned char>& data)
{
    unsigned long hash = 5381;

    for (size_t i = 0; i < data.size(); i++)
    {
        hash = ((hash << 5) + hash) + data[i];
    }

    std::stringstream ss;
    ss << std::hex << std::setw(16) << std::setfill('0') << hash;
    return ss.str();
}
