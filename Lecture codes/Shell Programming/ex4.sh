#!/bin/bash
echo -n "Enter file name> "
read file
# Use elif in bash for the else if.
# >> in the example is output redirection(append).
# The ls output will be appended to the file.
if [ -w "$file" ]; then
ls >> $file
echo "More input has been appended"
elif [ -e "$file" ]; then
echo "You have no write permission on $file"
else
echo "$file does not exist"
fi
