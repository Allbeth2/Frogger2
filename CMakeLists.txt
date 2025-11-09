cmake_minimum_required(VERSION 3.10)

project(Frogger2)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Define an executable
add_executable(Frogger2 ${SOURCES})

# Include directories
include_directories("src")

if(WIN32)
    # Windows-specific configuration
    include_directories(
        "${CMAKE_SOURCE_DIR}/libs/SDL3/include"
        "${CMAKE_SOURCE_DIR}/libs/SDL3_image/include"
    )
    link_directories(
        "${CMAKE_SOURCE_DIR}/libs/SDL3/lib"
        "${CMAKE_SOURCE_DIR}/libs/SDL3_image/lib"
    )
endif()

# Link libraries
target_link_libraries(Frogger2 PRIVATE SDL3 SDL3_image)