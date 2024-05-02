#!/bin/bash

stat mnt_pt || { echo "mnt_pt does not exists"; exit 0; }

ORIGINAL_PATH=$PATH
export PATH="external/fuse/build/util:$PATH"

umount mnt_pt || { echo "Failed to unmnt mongo_fuse_fs"; exit 1; }

export PATH=$ORIGINAL_PATH

