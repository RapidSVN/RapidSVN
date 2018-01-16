#! /bin/bash
# cd to script path
cd "${0%/*}"

./beautify.sh ../libsvncpp/include/svncpp/*.hpp
./beautify.sh ../libsvncpp/src/*.cpp
./beautify.sh ../librapidsvn/include/*.hpp
./beautify.sh ../librapidsvn/src/*.cpp
./beautify.sh ../rapidsvn/*.cpp
