#!/bin/bash 

echo Enter One for ls, Two for ls -1 and Three for ls -l 
read option

case $option in 

"One") echo you selected ls
   ls;;

"Two") echo you selected ls -1
   ls -1 ;;

"Three") echo you selected ls -l
   ls -l ;;

*) echo sorry, could not find an option;;

esac



