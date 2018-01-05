#!/bin/bash

# Récupération des données
read N
for (( i=0; i<N; i++ )); do
    read tab[i]
done

tab=($(for each in ${tab[@]}; do echo $each; done | sort -n))

D=10000000
for (( i=0 ; i<$((N - 1)) ; i++))
do
    current=${tab[i]}
    next=${tab[i+1]}
    temp=$(($next  - $current))
    #temp=$((${tab[i+1]} - ${tab[i]}))
    if [ $temp -lt $D ]
    then
        D=$temp
    fi
done

echo "$D"
