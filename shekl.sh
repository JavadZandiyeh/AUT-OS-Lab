#!/bin/bash

PS3="pick a number: "
select num in 1 2 3; do
    case $num in 
        1) 
            for var in $(seq 1 5)
            do
                for i in $(seq 1 $var)
                do
                    printf "%d" $var
                done
                printf "\n"
            done 
			break ;;
        2) 
			for var in $(seq 1 6)
            do
                let space=5-$var
                for var1 in $(seq 0 $space)
                do
                    printf " "
                done
                for var2 in $(seq 1 $var)
                do
                    printf "* "
                done
                printf "\n"
            done

			for var in $(seq 1 6)
            do
                let var=7-$var
                let space=5-$var
                for var1 in $(seq 0 $space)
                do
                    printf " "
                done
                for var2 in $(seq 1 $var)
                do
                    printf "* "
                done
                printf "\n"
            done

			break ;;
        3) 
			for var in $(seq 0 4)
            do
                for var1 in $(seq 1 $var)
                do  
                    printf "| "
                done
                printf "|_\n"
            done
			break ;;
        *) 
			echo "wrong input!!!!" 
			;;
    esac
done