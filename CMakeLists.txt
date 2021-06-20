cmake_minimum_required(VERSION 3.0)
project(FlvParser)

set(CMAKE_CXX_STANDARD 11)
include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(flvparser)
add_subdirectory(test)