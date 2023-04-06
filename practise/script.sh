#!/bin/bash

dir="/bin/"

for BB in "$dir"* ; do
  if [ -f "$BB" ] ; then
    chmod +x "$BB"
  fi
done