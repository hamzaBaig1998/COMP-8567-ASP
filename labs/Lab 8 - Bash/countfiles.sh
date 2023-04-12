#!/bin/bash

# Check if the correct number of arguments have been passed
if [ $# -lt 1 ]; then
  echo "Usage: $0 <directory_path> [<extension>]"
  exit 1
fi

dir_path=$1
extension=$2

# Count the number of files with the given extension in the directory
if [ -z "$extension" ]; then
  count=$(find "$dir_path" -type f | wc -l)
else
  count=$(find "$dir_path" -type f -name "*$extension" | wc -l)
fi

# Display the count
if [ $count -eq 0 ]; then
  if [ -z "$extension" ]; then
    echo "No files found in directory $dir_path"
  else
    echo "No files found with extension $extension in directory $dir_path"
  fi
else
  if [ -z "$extension" ]; then
    echo "Number of files in directory $dir_path: $count"
  else
    echo "Number of files with extension $extension in directory $dir_path: $count"
  fi
fi


