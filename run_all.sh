#!/bin/bash

echo "waiting of mongodb server"
until mongosh --eval "print(\"waited for connection\")"
  do
    sleep 60
done

echo "dropping fs_root database"
mongosh --eval "use fs_root" --eval  "db.dropDatabase()"
echo "creating db fs_root"
mongosh --eval "use blog" 
./run_mongo_fs.sh
./run_driver.sh
