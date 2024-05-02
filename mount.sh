#!/bin/bash

./umount.sh
if [ -d mnt ] 
then
  echo "dir mnt exists"
else
  echo "creating mnt dir"
  mkdir mnt
fi

echo "mounting mongo_fuse_fs to mnt"
./build/mongo_fuse_fs -f mnt > filesystem.log 2>&1 &
