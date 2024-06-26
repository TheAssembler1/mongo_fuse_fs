cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME mongo_fuse_fs)
project(${PROJECT_NAME})

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")

set(EXTERNAL_LIBS
  "${CMAKE_SOURCE_DIR}/external/fuse3/install/lib/x86_64-linux-gnu/libfuse3.so"
  "${CMAKE_SOURCE_DIR}/external/mongo-cxx-driver-r3.10.1/install/lib/libmongocxx.so"
  "${CMAKE_SOURCE_DIR}/external/mongo-cxx-driver-r3.10.1/install/lib/libbsoncxx.so"
)

set(EXTERNAL_HEADERS
  "${CMAKE_SOURCE_DIR}/external/fuse3/install/include/fuse3"
  "${CMAKE_SOURCE_DIR}/external/mongo-cxx-driver-r3.10.1/install/include/mongocxx/v_noabi"
  "${CMAKE_SOURCE_DIR}/external/mongo-cxx-driver-r3.10.1/install/include/bsoncxx/v_noabi"
)

# Add your source files
set(SOURCES
    src/entry.cpp
    src/fs/handler.cpp
    src/fs/perm.cpp
    src/fs/fs_helper.cpp
    src/mongo/mongo_manager.cpp
    src/mongo/collection/fs_metadata_collection.cpp
    src/mongo/collection/fs_data_collection.cpp
    src/mongo/collection/fs_lookup_collection.cpp
    src/mongo/collection/collection_helper.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include FUSE headers
target_include_directories(${PROJECT_NAME} PRIVATE "${EXTERNAL_HEADERS}")

# Link FUSE libraries
target_link_libraries(${PROJECT_NAME} PRIVATE ${EXTERNAL_LIBS})

