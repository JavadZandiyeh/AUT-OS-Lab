#!/bin/bash

read -r -p "Enter your number: " n
baghimande=0
rev_n=0
sum=0
while [ $n -gt 0 ]
do
    baghimande=$(( $n % 10 ))
    sum=$(( $sum + $baghimande ))
    rev_n=$(( $rev_n * 10 + $baghimande ))
    n=$(( $n / 10 ))
done
echo "revers: $rev_n"
echo "summation of digits: $sum"