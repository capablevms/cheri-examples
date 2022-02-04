#!/bin/bash
# vim: expandtab sts=0 sw=4 smarttab
#
# Copyright (c) 2020-2021 The CapableVMs "CHERI Examples" Contributors.
# SPDX-License-Identifier: MIT OR Apache-2.0

set -e

export CHERIBASE=~/cheri
CHERIBUILD=~/build
export TEST_TARGET=$1

echo "Checking clang-format..."
# Note that `sdk` and `morello-sdk` contain different versions of clang-format,
# so we have to pick one and use it consistently.
CLANG_FORMAT="$CHERIBASE"/output/sdk/bin/clang-format
find . -iname "*.c" -o -iname "*.h" -o -iname "*.cpp" -o -iname "*.hpp" | xargs "$CLANG_FORMAT" --dry-run -Werror

echo "Checking that all the purecap examples build for all platforms..."
PLATFORMS='riscv64-purecap morello-purecap'
# TODO: Add "example_allocators".
for dir in . employee shared_objects timsort; do
    pushd "$dir"
    for platform in $PLATFORMS; do
        make -f Makefile.$platform clean
        make -f Makefile.$platform all
    done
    popd
done

echo "Checking that all the hybrid examples build on Morello..."
platform='morello-hybrid'
for dir in hybrid hybrid/ddc_compartment_switching; do
    pushd "$dir"
    make -f Makefile.$platform clean
    make -f Makefile.$platform all
    popd
done

export SSHPORT=10021
export PYTHONPATH="$CHERIBUILD"/test-scripts

if [ "$1" = "riscv64" ]; then
    echo "Running tests for 'riscv64-purecap' using QEMU."
    args=(
        --architecture riscv64
        # Qemu System to use
        --qemu-cmd $HOME/cheri/output/sdk/bin/qemu-system-riscv64cheri
        --kernel $HOME/cheri/output/rootfs-riscv64-purecap/boot/kernel/kernel
        --bios bbl-riscv64cheri-virt-fw_jump.bin
        --disk-image $HOME/cheri/output/cheribsd-riscv64-purecap.img
        # Required build-dir in CheriBSD
        --build-dir .
        --ssh-port $SSHPORT
        --ssh-key $HOME/.ssh/id_ed25519.pub
    )
    BUILDBOT_PLATFORM=riscv64-purecap python3 tests/run_cheri_examples.py "${args[@]}"
elif [ "$1" = "morello-hybrid" ]; then
    echo "Running tests for 'morello-hybrid' using QEMU..."
    args=(
        --architecture morello-hybrid
        # Qemu System to use
        --qemu-cmd $HOME/cheri/output/morello-sdk/bin/qemu-system-morello
        --bios edk2-aarch64-code.fd
        --disk-image $HOME/cheri/output/cheribsd-morello-hybrid.img
        # Required build-dir in CheriBSD
        --build-dir .
        --ssh-port $SSHPORT
        --ssh-key $HOME/.ssh/id_ed25519.pub
    )
    BUILDBOT_PLATFORM=morello-hybrid python3 tests/run_cheri_examples.py "${args[@]}"
else
    echo "$1 not supported (yet)."
    exit 1
fi

# TODO: Run tests for `morello-purecap` and `riscv64-purecap` too
# This is related to https://github.com/CTSRD-CHERI/cheribuild/issues/182
