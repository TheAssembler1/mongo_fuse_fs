#!/bin/bash

echo "removing apt packages"
sudo apt remove libfuse3-dev 
echo "installing apt packages"
sudo apt install cmake meson build-essential || { echo "Failed to install apt packages"; exit 1; }
echo "removing external dir"
sudo rm -rf external

echo "creating and moving int external dir"
mkdir external && cd external

echo "installing tar packages"
FUSE3_FOLDER_NAME=fuse3
FUSE3_FILENAME_TAR="$FUSE3_FOLDER_NAME.tar.gz"
FUSE3_FOLDER_ABS_NAME="$(pwd)/$FUSE3_FOLDER_NAME/install"
curl -L --output $FUSE3_FILENAME_TAR "https://github.com/libfuse/libfuse/releases/download/fuse-3.16.2/fuse-3.16.2.tar.gz"
mkdir $FUSE3_FOLDER_NAME && tar xf $FUSE3_FILENAME_TAR -C $FUSE3_FOLDER_NAME --strip-components 1 || { echo "Failed to extract and rename fuse3 tar"; exit 1; }
cd $FUSE3_FOLDER_NAME || { echo "Failed to move into fuse3 dir"; exit 1; }
mkdir build && cd build
mkdir $FUSE3_FOLDER_ABS_NAME 

echo "moving build dir"
meson ..
echo "configuring meson"
meson configure --prefix=$FUSE3_FOLDER_ABS_NAME || { echo "Failed to configure build"; exit 1; }
echo "building project with ninja"
ninja || { echo "Failed to build with ninja"; exit 1; }
echo "installing project with ninja"
sudo ninja install || { echo "Failed to install with ninja"; exit 1; }

