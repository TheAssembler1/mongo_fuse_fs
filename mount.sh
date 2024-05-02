#!/bin/bash

ORIGINAL_PATH=$PATH
export PATH="external/fuse/build/util:$PATH"

./umount.sh
mkdir mnt_pt 
mount -t fuse -f -s build/mongo_fuse_fs mnt_pt || { echo "Failed to mnt mongo_fuse_fs"; exit 1; }

export PATH=$ORIGINAL_PATH
