#!/bin/bash
array=('a' 'b' 'c' 123 456 789 1234 "asd" "%^&*" "kek")

for i in ${array[@]}
do
    echo $i
done