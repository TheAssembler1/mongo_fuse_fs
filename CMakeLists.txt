cmake_minimum_required(VERSION 3.0)
set(PROJECT_NAME mongo_fuse_fs)
project(${PROJECT_NAME})

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Get compiler flags from pkg-config
execute_process(
    COMMAND pkg-config fuse3 --cflags
    OUTPUT_VARIABLE FUSE_CFLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Get linker flags from pkg-config
execute_process(
    COMMAND pkg-config fuse3 --libs
    OUTPUT_VARIABLE FUSE_LIBS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FUSE_CFLAGS}")

# Set linker flags
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${FUSE_LIBS}")

# Add your source files
set(SOURCES
    src/entry.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${FUSE_LIBS})

