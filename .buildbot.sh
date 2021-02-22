#!/bin/bash
# vim: expandtab sts=0 sw=4 smarttab
set -e

mkdir -p ./bin
for example in *.c; do
    if [ "${example}" == "seal.c" ]; then
        continue
    fi
    make bin/"${example%%.*}"
done

mkdir build 
cd build
    
cmake -DCMAKE_TOOLCHAIN_FILE=riscv64-purecap.cmake ..
make

cd ..
rm -rfv build
