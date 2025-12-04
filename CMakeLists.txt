cmake_minimum_required(VERSION 3.5.0)
project(raytracer VERSION 0.1.0 LANGUAGES C CXX)
option(ENABLE_THREADING "Enable multithreading for rendering")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Optimization flags for performance
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")

add_executable(raytracer main.cpp)

if(ENABLE_THREADING)
    add_compile_definitions(ENABLE_THREADING) 
    message(STATUS "Multithreading: ENABLED")
else()
    message(STATUS "Multithreading: DISABLED")
endif()

target_include_directories(raytracer PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_SOURCE_DIR}/src/raymath"
                           "${PROJECT_SOURCE_DIR}/src/rayimage"
                           "${PROJECT_SOURCE_DIR}/src/rayscene"
                           )

add_subdirectory(./src/raymath)
add_subdirectory(./src/rayimage)
add_subdirectory(./src/rayscene)
add_subdirectory(./src/lodepng)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(raytracer
                      PRIVATE Threads::Threads
                      PUBLIC
                      rayscene
                      raymath
                      rayimage
                      lodepng
                      )

enable_testing()
add_subdirectory(tests)

