cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME mongo_fuse_fs)
project(${PROJECT_NAME})

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Set FUSE libraries and headers
set(FUSE_LIBS "${CMAKE_SOURCE_DIR}/external/fuse3/install/lib/x86_64-linux-gnu/libfuse3.so")
set(FUSE_HEADERS "${CMAKE_SOURCE_DIR}/external/fuse3/install/include/fuse3")

# Add your source files
set(SOURCES
    src/entry.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include FUSE headers
target_include_directories(${PROJECT_NAME} PRIVATE "${FUSE_HEADERS}")

# Link FUSE libraries
target_link_libraries(${PROJECT_NAME} PRIVATE ${FUSE_LIBS})

