
#!/bin/bash
counter=$1
factorial=1

until [ $counter == 0 ]
do
   factorial=$(( $factorial * $counter ))
   counter=$(( $counter - 1 ))
done
echo $factorial
