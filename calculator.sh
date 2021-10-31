#!/bin/bash

re='^[0-9]+$'
while true; do
	# enter first number
    read -r -p "Enter first number: " a
	while ! [[ $a =~ $re ]]
	do
		read -r -p 'please enter a number nothing else: ' a
	done

	# enter second number
	read -r -p 'Enter second number: ' b
	while ! [[ $b =~ $re ]]
	do
        read -r -p 'please enter a number nothing else: ' b 
	done

    PS3="choos operation please: "
    select ans in add subtract multiply divide; do
        case $ans in 
            add) 
				op='+' ; 
				break ;;
            subtract) 
				op='-' ;
				break ;;
            multiply) 
				op='*' ;
				break ;;
            divide) 
				op='/' ;
				break ;;
            *) 
				echo "whyyyyyy:)" 
				;;
        esac
    done
    ans=$(echo "$a $op $b" | bc -l)
    printf "%s %s %s = %s\n\n" "$a" "$op" "$b" "$ans"
done