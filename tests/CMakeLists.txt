cmake_minimum_required(VERSION 3.5.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(
    ${PROJECT_SOURCE_DIR}/src/raymath
    ${PROJECT_SOURCE_DIR}/src/rayimage
    ${PROJECT_SOURCE_DIR}/src/rayscene
    ${PROJECT_SOURCE_DIR}/src/lodepng
    ${PROJECT_SOURCE_DIR}/tests/utils
)

add_library(test_utils
    utils/image_comparison.cpp
    utils/performance_metrics.cpp
    utils/test_fixture.cpp
)

target_link_libraries(test_utils
    PRIVATE Threads::Threads
    PUBLIC
    rayscene
    raymath
    rayimage
    lodepng
)

add_executable(test_simple_sphere standard/test_simple_sphere.cpp)
target_link_libraries(test_simple_sphere test_utils)
add_test(NAME SimpleSphereTwoOnPlane COMMAND test_simple_sphere)

add_executable(test_complex_geometry standard/test_complex_geometry.cpp)
target_link_libraries(test_complex_geometry test_utils)
add_test(NAME ComplexGeometryMonkey COMMAND test_complex_geometry)

add_executable(test_multi_object standard/test_multi_object.cpp)
target_link_libraries(test_multi_object test_utils)
add_test(NAME MultiObjectGalaxy COMMAND test_multi_object)

add_executable(test_boundary_conditions edge_cases/test_boundary_conditions.cpp)
target_link_libraries(test_boundary_conditions test_utils)
add_test(NAME BoundaryConditions COMMAND test_boundary_conditions)

add_executable(test_numerical_precision edge_cases/test_numerical_precision.cpp)
target_link_libraries(test_numerical_precision test_utils)
add_test(NAME NumericalPrecision COMMAND test_numerical_precision)

add_executable(test_degenerate_cases edge_cases/test_degenerate_cases.cpp)
target_link_libraries(test_degenerate_cases test_utils)
add_test(NAME DegenerateCases COMMAND test_degenerate_cases)

add_executable(test_regression_sphere standard/test_regression_sphere.cpp)
target_link_libraries(test_regression_sphere test_utils)
add_test(NAME RegressionSphereIntersection COMMAND test_regression_sphere)
