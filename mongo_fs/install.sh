#!/bin/bash

ORIGINAL_DIR=$(pwd)

echo "stopping services"
sudo service mongod stop
echo "removing apt packages"
sudo apt remove libfuse3-dev 
sudo apt purge "mongodb-org*"
echo "installing apt packages"
sudo apt install libssl-dev make meson build-essential gnupg || { echo "Failed to install apt packages"; exit 1; }
echo "removing dirs"
sudo rm -rf external
sudo rm -rf /var/log/mongodb
sudo rm -rf /var/lib/mongodb

echo "creating and moving int external dir"
mkdir external && cd external

echo "installing tar packages"
FUSE3_FOLDER_NAME=fuse3
FUSE3_FILENAME_TAR="$FUSE3_FOLDER_NAME.tar.gz"
FUSE3_FOLDER_ABS_NAME="$ORIGINAL_DIR/external/$FUSE3_FOLDER_NAME/install"
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
cd $ORIGINAL_DIR

cd external
echo "install mongodb"
curl -fsSL https://www.mongodb.org/static/pgp/server-7.0.asc | \
   sudo gpg -o /usr/share/keyrings/mongodb-server-7.0.gpg \
   --dearmor
echo "deb [ arch=amd64,arm64 signed-by=/usr/share/keyrings/mongodb-server-7.0.gpg ] https://repo.mongodb.org/apt/ubuntu jammy/mongodb-org/7.0 multiverse" | sudo tee /etc/apt/sources.list.d/mongodb-org-7.0.list
sudo apt-get update
sudo apt-get install -y mongodb-org
echo "mongodb-org hold" | sudo dpkg --set-selections
echo "mongodb-org-database hold" | sudo dpkg --set-selections
echo "mongodb-org-server hold" | sudo dpkg --set-selections
echo "mongodb-mongosh hold" | sudo dpkg --set-selections
echo "mongodb-org-mongos hold" | sudo dpkg --set-selections
echo "mongodb-org-tools hold" | sudo dpkg --set-selections
cd $ORIGINAL_DIR

cd external
echo "install mongo compass"
wget https://downloads.mongodb.com/compass/mongodb-compass_1.42.5_amd64.deb
sudo apt install ./mongodb-compass_1.42.5_amd64.deb
cd $ORIGINAL_DIR

cd external
MONGO_FOLDER_NAME="mongo-cxx-driver-r3.10.1"
MONGO_FOLDER_ABS_NAME="$ORIGINAL_DIR/external/$MONGO_FOLDER_NAME/install"
echo "install mongo c++ driver"
curl -OL https://github.com/mongodb/mongo-cxx-driver/releases/download/r3.10.1/mongo-cxx-driver-r3.10.1.tar.gz
tar -xzf "$MONGO_FOLDER_NAME.tar.gz"
mkdir $MONGO_FOLDER_ABS_NAME && cd "$MONGO_FOLDER_NAME/build"
cmake ..                                \
    -DCMAKE_BUILD_TYPE=Release          \
    -DCMAKE_INSTALL_PREFIX=$MONGO_FOLDER_ABS_NAME \
    -DCMAKE_CXX_STANDARD=17 \
    -DCMAKE_INSTALL_RPATH=$MONGO_FOLDER_ABS_NAME/lib
cmake --build .
sudo cmake --build . --target install
cd $ORIGINAL_DIR
