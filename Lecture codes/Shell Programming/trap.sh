#!/bin/bash
# trap.sh
# notice you cannot make Ctrl-C work in this shell becuase it has been trapped

trap "echo That does not work over here" SIGINT
echo "The script is going to run until you hit Ctrl+Z"
echo "Try CTRL+C if you want to"


while [ true ]        
do
  sleep 3         
done
