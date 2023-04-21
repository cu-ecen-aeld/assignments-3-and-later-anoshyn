#!/bin/bash

# Check that both arguments were provided
if [ "$#" -lt 2 ]; then
  echo "Usage: $0 <writefile> <writestr>"
  exit 1
fi

# Get the writefile and writestr arguments
writefile=$1
writestr=$2

# Create the directory if it does not exist
dir=$(dirname "$writefile")
if [ ! -d "$dir" ]; then
  mkdir -p "$dir" || { echo "Could not create directory $dir"; exit 1; }
fi

# Write the content to the file
echo "$writestr" > "$writefile" || { echo "Could not write to file $writefile"; exit 1; }

# Print a success message
echo "File $writefile created with content:"
echo "$writestr"

