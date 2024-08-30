cmake_minimum_required(VERSION 3.8.0)
project (TwoMovingAverageQRSdetector LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wconversion -Wextra -pedantic)

add_executable(demo demo.cpp)
target_link_libraries(demo iir)

enable_testing()
add_executable(test_avg test_avg.cpp)
add_test(TestMovingAverage test_avg)
