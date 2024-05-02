#!/bin/bash

cmake -Bbuild || { echo "Failed to build project"; exit 1; }
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON build || { echo "Failed to create compile commands"; exit 1; }
cp build/compile_commands.json . || { echo "Failed to copy compile commands"; exit 1; }
