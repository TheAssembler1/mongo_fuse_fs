#!/bin/bash

echo "preparint mnt point"
fusermount -u mnt || true
rmdir mnt || true
mkdir mnt || true

echo "waiting of mongodb server"
until mongosh --eval "print(\"waited for connection\")"
  do
    sleep 60
done

echo "dropping fs_root database"
mongosh --eval "use fs_root" --eval  "db.dropDatabase()"
echo "creating db fs_root"
mongosh --eval "use blog" 

echo "mounting mongo_fuse_fs to mnt"
rm mongo_fuse_fs.output || true
./mongo_fs/build/mongo_fuse_fs -f mnt
