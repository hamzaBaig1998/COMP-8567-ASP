#!/bin/bash
#ex1.sh

echo -n "Enter a value> "
read a
echo -n "Enter another value> "
read b
echo "Doing arithmetic> "
# surround expressions with $(( expression )).
sum=$(( $a + $b ))
echo "The sum $a + $b is $sum"
difference=$(( $a - $b ))
echo "The difference $a - $b is $difference"
product=$(($a * $b))
echo "The product $a * $b is $product"

if [[ $b -ne 0 ]]; then
quotient=$(($a / $b))
echo "The division $a / $b is $quotient"
else
echo "The division $a/$b is impossible"
fi
