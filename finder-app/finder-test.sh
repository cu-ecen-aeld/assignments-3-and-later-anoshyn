#!/bin/bash
# Tester script for finder.c
# Author: Siddhant Jajoo

set -e
set -u

WRITEDIR=/tmp/aeld-data
username=$(cat ../conf/username.txt)

echo "Removing old build artifacts"
make clean

echo "Compiling writer application"
make

echo "Writing files using writer application"
./writer "$WRITEDIR/${username}1.txt" "test string 1"
./writer "$WRITEDIR/${username}2.txt" "test string 2"
./writer "$WRITEDIR/${username}3.txt" "test string 3"

echo "Searching for files containing 'test string'"
OUTPUTSTRING=$(./finder.sh "$WRITEDIR" "test string")

echo "Search output:"
echo "${OUTPUTSTRING}"

# echo "Cleaning up temporary files"
# rm -rf "${WRITEDIR}"

