#!/bin/bash

echo "=========================================="
echo "Generating reference images for tests"
echo "Using ORIGINAL scenes (full resolution)"
echo "=========================================="
echo ""

mkdir -p tests/fixtures/reference_images

echo "1/3 Generating two-spheres-on-plane.png (1920x1080)..."
./build/raytracer scenes/two-spheres-on-plane.json tests/fixtures/reference_images/two-spheres-on-plane.png

echo "2/3 Generating monkey-on-plane.png (1920x1080)..."
./build/raytracer scenes/monkey-on-plane.json tests/fixtures/reference_images/monkey-on-plane.png

echo "3/3 Generating sphere-galaxy-on-plane.png (1920x1080)..."
./build/raytracer scenes/sphere-galaxy-on-plane.json tests/fixtures/reference_images/sphere-galaxy-on-plane.png

echo ""
echo "=========================================="
echo "Reference images generated successfully!"
echo "=========================================="
echo ""
echo "Generated files:"
ls -lh tests/fixtures/reference_images/
echo ""
echo "✅ Tests are now ready to run!"
echo ""
echo "Run all tests:        cd build && ctest"
echo "Run tests (verbose):  cd build && ctest --verbose"
echo ""
echo "⚠️  Note: Tests will take ~6 minutes (original scenes are heavy)"
