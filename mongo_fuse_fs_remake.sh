#!/bin/bash

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B build_mongo_fuse_fs -C mongo_fs/CMakeLists.txt || { echo "Failed to create compile commands"; exit 1; }
cp build_mongo_fuse_fs/compile_commands.json . || { echo "Failed to copy compile commands"; exit 1; }
