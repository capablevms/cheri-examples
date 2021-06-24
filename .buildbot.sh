#!/bin/bash
# vim: expandtab sts=0 sw=4 smarttab
set -e

find . -iname "*.c" -o -iname "*.h" -o -iname "*.cpp" -o -iname "*.hpp" | xargs ~/cheri/output/sdk/bin/clang-format --dry-run -Werror

mkdir -p ./bin
for example in *.c; do
    if [ "${example}" == "seal.c" ]; then
        continue
    fi
    make -f Makefile.riscv64 bin/"${example%%.*}"
done

# arg-1 : Source directory
# arg-2 : cmake toolchain file
function build_cheri_examples()
{
    ARCH=$(echo ${2} | cut -d '.' -f 1)
    BUILD_DIR=${1}/build/${ARCH}

    mkdir -p ${BUILD_DIR}
    cmake -B ${BUILD_DIR} -S ${1}  -DCMAKE_TOOLCHAIN_FILE=${2}
    cmake --build ${BUILD_DIR}
}

build_cheri_examples $(pwd) riscv64-purecap.cmake
build_cheri_examples $(pwd) morello-purecap.cmake
rm -rfv $(pwd)/build
