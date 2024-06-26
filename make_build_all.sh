#!/bin/bash

ORIGINAL_DIR=$(pwd)

fail() {
  echo "$1"
  exit 1
}

make_build_project() {
  echo "remaking $1"
  cd $1 
  ./remake.sh || fail "Failed to remake $1"
  echo "rebuilding $1"
  ./rebuild.sh || fail "Failed to rebuild $1"
  cd $ORIGINAL_DIR
}

make_build_project "mongo_fs"
make_build_project "driver"

