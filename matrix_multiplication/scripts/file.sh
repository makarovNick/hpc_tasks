#!/bin/bash

name=Linux

if [ -f "$name" ]; then
    echo "course"
else
    echo "very easy"
    echo "course is easy" > $name
fi