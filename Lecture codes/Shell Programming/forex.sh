
#!/bin/bash

echo For loop with an explicit list 

for k in SUN MON WED TUE FRI
do
for m in {10..30}
do
for j in {100..200..20}
do
i=$(($j+1))
done
done
done
echo $((m*i))




# echo For Loop with range and default increment of 1

# for i in {1..10}
# do
# echo $i
# done


# echo For loop with increments of 2 

# for i in {1..10..2}
# do
# echo $i
# done



