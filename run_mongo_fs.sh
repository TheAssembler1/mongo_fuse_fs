#!/bin/bash

echo "preparint mnt point"
fusermount -u mnt || true
rmdir mnt || true
mkdir mnt || true


echo "mounting mongo_fuse_fs to mnt"
gnome-terminal -- /bin/bash -c './mongo_fs/build/mongo_fuse_fs -f mnt'
