#!/bin/bash

if [ $# -eq 0 ]; then
    echo "Error: no input files"
    exit 1
fi

for file in "$@"; do
    if [ ! -f "$file" ]; then
        echo "Error: file $file not found"
        exit 1
    elif [[ ! "$file" == *.txt ]]; then
        echo "Error: file $file is not a .txt file"
        exit 1
    fi
done

rm -f output_rev.txt
for ((i=$#; i>=1; i--)); do
    cat "${!i}" >> output_rev.txt
done

echo "Concatenation complete. Results saved in output_rev.txt"
