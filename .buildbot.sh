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

# Copy the test-script to `cheribuild`
cp ./tests/run_cheri_examples.py $HOME/build/test-scripts/.

# Run the tests on the `qemu-system riscv64`
args=(
    # Architecture
    --architecture riscv64
    # Qemu System to use
    --qemu-cmd $HOME/cheri/output/sdk/bin/qemu-system-riscv64cheri
    # Kernel (to avoid the default one)
    --kernel $HOME/cheri/output/rootfs-riscv64-purecap/boot/kernel/kernel
    # Bios (to avoid the default one)
    --bios bbl-riscv64cheri-virt-fw_jump.bin
    # Disk Image
    --disk-image $HOME/cheri/output/cheribsd-riscv64-purecap.img
    # Required build-dir in CheriBSD
    --build-dir .
    --ssh-port 10021
    --ssh-key $HOME/.ssh/id_ed25519.pub
    )

python3 $HOME/build/test-scripts/run_cheri_examples.py "${args[@]}"
rm -rfv bin/*

BUILD_DIR="bin"

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

# FIXME: EXCLUDING MORELLO to close PR#26
# build_cheri_examples $(pwd) morello-purecap.cmake
rm -rfv $(pwd)/build
