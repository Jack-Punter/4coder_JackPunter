#!/bin/sh
#@echo off

if [ ! -d "./build" ]
then 
    echo
else
    mkdir build
fi

cd build/

../../bin/buildsuper_x64-linux.sh ../JackPunter.cpp

#echo End of compile
cp custom_4coder.so ../../../custom_4coder.so
