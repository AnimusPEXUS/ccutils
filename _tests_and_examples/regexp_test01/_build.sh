#!/bin/bash

cmake -S . -B ./build
if [ $? -ne 0 ]
then
    exit 0
fi

#cd ./build && cmake "$@"
cmake --build ./build

