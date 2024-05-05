cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME driver)
project(${PROJECT_NAME})

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")

# Add your source files
set(SOURCES
    src/entry.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

