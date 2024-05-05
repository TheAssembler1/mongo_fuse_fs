#!/bin/bash

if [ -d mnt ] 
then
  echo "dir mnt exists"
else
  echo "creating mnt dir"
  mkdir mnt
fi

if [ -d mnt ]
then
  echo "found mnt dir"
  echo "unmounting mnt dir"
  fusermount -u mnt || true
  rmdir mnt || true
else
  echo "no mnt dir found"
fi

echo "creating mnt dir"
mkdir mnt


echo "mounting mongo_fuse_fs to mnt"
./mongo_fs/build/mongo_fuse_fs -f mnt
