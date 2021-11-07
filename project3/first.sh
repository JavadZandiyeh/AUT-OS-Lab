#!/bin/bash

# regular expretion to chech that the input is a number or not
re='^[0-9]+$'

# get first number
read -r -p 'Enter first number: ' a
while ! [[ $a =~ $re ]]
do
	read -r -p 'please enter a number nothing else: ' a
done

# ger second number
read -r -p 'Enter second number: ' b
while ! [[ $b =~ $re ]]
do
        read -r -p 'please enter a number nothing else: ' b 
done

# compute sum
(( sum=a+b ))
echo "sum: $sum"

# find which one is greater
if [[ $a -gt $b ]]
then
	echo "$a is greater than $b"
else
	echo "$b is greater than $a"
fi

