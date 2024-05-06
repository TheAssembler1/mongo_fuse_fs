#!/bin/bash

ORIGINAL_DIR=$(pwd)

fail() {
  echo "$1"
  exit 1
}

build_project() {
  cd $1
  echo "rebuilding $1"
  ./rebuild.sh || fail "Failed to rebuild $1"
  cd $ORIGINAL_DIR
}

build_project "mongo_fs"
build_project "driver"

