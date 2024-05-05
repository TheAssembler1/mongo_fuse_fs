cd build || { echo "Failed to move to build dir"; exit 1; }
make clean
make || { echo "Failed to build mongo_fuse_fs"; exit 1; }

