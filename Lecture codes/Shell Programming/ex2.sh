#!/bin/bash
if [ $# != 2 ];then
echo "Usage: $0 integer1 integer2"
else
echo Doing arithmetic
r=$(($1 + $2))
echo "the sum $1 + $2 is $r"
r=$(($1 - $2))
echo "the subtraction $1 - $2 is $r"
r=$(($1 * $2))
echo "the product $1 * $2 is $r"
if [ $2 -ne 0 ] ; then
r=$(($1 / $2))
echo "the division $1 / $2 is $r"
else
echo "the division $1 / $2 is impossible"
fi
fi
