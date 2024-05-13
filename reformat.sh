#!/bin/bash

echo "formatting driver"
find driver/src -iname '*.h' -o -iname '*.cpp' | xargs clang-format -i -style=file:"$(pwd)/.clang-format"

echo "formating mongo fuse fs"
find mongo_fs/src -iname '*.h' -o -iname '*.cpp' | xargs clang-format -i -style=file:"$(pwd)/.clang-format"
