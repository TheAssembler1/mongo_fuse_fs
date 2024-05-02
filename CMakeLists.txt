cmake_minimum_required(VERSION 3.0)
set(PROJECT_NAME mongo_fuse_fs)
project(${PROJECT_NAME})

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# external dependencies path
set(FUSE_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/external/fuse/build/lib/libfuse3.so")
set(FUSE_HEADERS "${CMAKE_CURRENT_LIST_DIR}/external/fuse/build")

# Add include directories
include_directories(${FUSE_HEADERS})

# Add your source files
set(SOURCES
    src/entry.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link against FUSE library
target_compile_definitions(${PROJECT_NAME} PRIVATE -D_FILE_OFFSET_BITS=64)
target_link_libraries(${PROJECT_NAME} ${FUSE_LIBRARIES})

