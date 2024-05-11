#!/bin/bash

rm driver.output || true
./driver/build/driver $(pwd)/mnt &> driver.output
