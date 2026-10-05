#!/bin/sh

mkdir -p build

g++ -std=c++17 \
    -Iinclude \
    src/main.cpp \
    src/centroid_decomposition.cpp \
    -o build/programa.exe

if [ $? -eq 0 ]; then
    echo "Compilación exitosa."
    ./build/programa.exe
else
    echo "Error de compilación."
    exit 1
fi