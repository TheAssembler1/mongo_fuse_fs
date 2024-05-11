#!/bin/bash

echo "preparint mnt point"
fusermount -u mnt || true
rmdir mnt || true
mkdir mnt || true


echo "mounting mongo_fuse_fs to mnt"
rm mongo_fuse_fs.output || true
./mongo_fs/build/mongo_fuse_fs -f mnt &> mongo_fuse_fs.output &
