#!/bin/bash

rm -rf external || { echo "Failed to remove external dir"; exit 1; }
mkdir external || { echo "Failed to create external dir"; exit 1; }
cd external || { echo "Failed to move into external dir"; exit 1; }
curl -L --output fuse.tar.gz "https://github.com/libfuse/libfuse/releases/download/fuse-3.16.2/fuse-3.16.2.tar.gz" || { echo "Failed to download fuse dependency"; exit 1; }
mkdir fuse || { echo "Failed to create fuse dir"; exit 1; }
tar -xzvf fuse.tar.gz -C fuse --strip-components=1 || { echo "Failed to unzip fuse dependency"; exit 1; }
cd fuse || { echo "Failed to move into fuse directory"; exit 1; }
mkdir build || { echo "Failed to create build dir"; exit 1; }
cd build || { echo "Failed to move into build dir"; exit 1; }
meson setup .. || { echo "Failed to configure build through meson"; exit 1; }
ninja || { echo "Failed to build fuse library with ninja"; exit 1; }

echo "Build completed successfully"
