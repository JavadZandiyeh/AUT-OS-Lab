#!/bin/bash

x=$1
y=$2
file_name=$3

counter=1
let x=x-1
let y=y+1

while IFS= read -r line; do
    if [[ $counter > $x  ]]; then
        if [[ $counter < $y ]]; then
            let counter=counter+1
            printf '%s\n' "$line"
        fi
    else
        let counter=counter+1
    fi
done < $file_name
