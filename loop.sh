#!/bin/bash

# check if the number of arguments is correct
if [ $# -ne 2 ]
then
    echo "Usage: $0 <number of times>"
    exit 1
fi  

# execute run.sh $1 times
for i in $(seq 1 $1)
do
    echo "Execution $i/$1"
    ./run.sh $2
done