#!/bin/bash

rm filesystem.log

if [ -d mnt ]
then
  echo "found mnt dir"
else
  echo "no mnt dir found"
  exit 0
fi

echo "unmounting mnt dir"
sudo umount mnt || { echo "Failed to unmnt mongo_fuse_fs"; exit 1; }
rmdir mnt
rm filesystem.log
