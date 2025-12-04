#!/bin/bash

echo "=========================================="
echo "Running QUICK tests (fast edge cases)"
echo "=========================================="
echo ""

cd build

ctest -R "Boundary|Numerical|Degenerate" --verbose

echo ""
echo "=========================================="
echo "Quick tests completed!"
echo ""
echo "To run ALL tests: ctest --verbose"
echo "=========================================="
