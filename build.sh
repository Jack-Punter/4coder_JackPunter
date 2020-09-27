#!bin/sh
#@echo off

mkdir build

cd build/

. ../../bin/buildsuper_x64-linux.sh ../JackPunter.cpp

#echo End of compile
cp custom_4coder.dll ../../../ustom_4coder.dll
