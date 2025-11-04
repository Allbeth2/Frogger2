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
include_directories(
    "libs/SDL3/include"
    "libs/SDL3_image/include"
    "src"
)

# Linker directories
link_directories(
    "libs/SDL3/lib"
    "libs/SDL3_image/lib"
)

# Link libraries
target_link_libraries(Frogger2 PRIVATE SDL3 SDL3_image)