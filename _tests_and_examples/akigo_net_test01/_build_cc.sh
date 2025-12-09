#!/bin/bash

#export CXX=`which clang++`
#export CC=`which clang`
#export 

cmake -S . -B ./build
if [ $? -ne 0 ]
then
    exit 0
fi

cmake --build ./build "$@"

