#!/bin/bash

#rm -r ./build

cmake -S . -B ./build
if [ $? -ne 0 ]
then
    exit 0
fi

cd ./build && make "$@"
