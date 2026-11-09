#!/bin/sh
set -eu

ROOT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
cd "$ROOT_DIR"

mkdir -p build output

EXE=""
case "$(uname -s 2>/dev/null || echo unknown)" in
    MINGW*|MSYS*|CYGWIN*) EXE=".exe" ;;
esac

CXX=${CXX:-g++}

"$CXX" -std=c++17 -O2 -DNDEBUG \
    -Iinclude -Igenerator \
    src/benchmark.cpp \
    generator/random_tree_generator.cpp \
    generator/star_tree_generator.cpp \
    generator/complete_tree_generator.cpp \
    generator/path_tree_generator.cpp \
    src/centroid_decomposition_folklore.cpp \
    src/graph_utils.cpp \
    src/centroid_decomposition_quasi_linear.cpp \
    -o "build/benchmark$EXE"

echo "Compilacion exitosa: build/benchmark$EXE"

ulimit -s unlimited 2>/dev/null || ulimit -s 1048576 2>/dev/null || ulimit -s 262144 2>/dev/null || true
echo "Stack limit: $(ulimit -s 2>/dev/null || echo desconocido)"

"./build/benchmark$EXE" "$@"
