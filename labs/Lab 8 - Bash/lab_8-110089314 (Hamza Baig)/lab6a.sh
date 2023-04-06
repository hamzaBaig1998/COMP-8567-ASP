#!/bin/bash

# Check if sample.txt exists in PWD
if [ -e sample.txt ]; then
  # Check if I am owner of the file
  if [ $(stat -c '%U' sample.txt) == $(whoami) ]; then
    # Check if the file has write permission
    if [ -w sample.txt ]; then
      # Append the contents of ls -1 into sample.txt
      ls -1 >> sample.txt
      echo "ls -1 output has been appended to sample.txt"
    else
      # Set the write permission of the file using chmod
      chmod 777 sample.txt
      echo "Write permission has been added to sample.txt"
      ls -1 >> sample.txt
      echo "ls -1 output has been appended to sample.txt"
    fi
  else
    # Output appropriate message if user is not the owner of the file
    echo "You are not the owner of sample.txt"
  fi
else
  # Create the file if it doesn't exist
  touch sample.txt
  echo "sample.txt has been created"
fi
