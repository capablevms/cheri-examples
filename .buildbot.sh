#!/bin/bash
# vim: expandtab sts=0 sw=4 smarttab
#
# Copyright (c) 2020-2022 The CapableVMs "CHERI Examples" Contributors.
# SPDX-License-Identifier: MIT OR Apache-2.0

set -e

echo "Checking clang-format..."
# Note that `sdk` and `morello-sdk` contain different versions of clang-format,
# so we have to pick one and use it consistently.
CLANG_FORMAT=$CHERI_DIR/morello-sdk/bin/clang-format
find . -iname "*.c" -o -iname "*.h" -o -iname "*.cpp" -o -iname "*.hpp" | xargs "$CLANG_FORMAT" --dry-run -Werror

echo "Checking that all riscv64-purecap examples build..."
# TODO: Add "example_allocators".
for dir in . employee shared_objects timsort; do
    pushd "$dir"
    make -f Makefile.riscv64-purecap clean
    make -f Makefile.riscv64-purecap all
    popd
done

echo "Checking that all morello-hybrid examples build..."
for dir in hybrid hybrid/ddc_compartment_switching syscall-restrict; do
    pushd "$dir"
    make -f Makefile.morello-hybrid clean
    make -f Makefile.morello-hybrid all
    popd
done

echo "Checking that all morello-purecap examples build..."
# TODO: Add "example_allocators".
for dir in . employee shared_objects timsort morello-c64-secure; do
    pushd "$dir"
        make -f Makefile.morello-purecap clean
        make -f Makefile.morello-purecap all
    popd
done

export SSHPORT=10021

echo "Running tests for 'riscv64-purecap' using QEMU."
args=(
    --architecture riscv64
    # Qemu System to use
    --qemu-cmd $CHERI_DIR/sdk/bin/qemu-system-riscv64cheri
    --kernel $CHERI_DIR/rootfs-riscv64-purecap/boot/kernel/kernel
    --bios bbl-riscv64cheri-virt-fw_jump.bin
    --disk-image $CHERI_DIR/cheribsd-riscv64-purecap.img
    # Required build-dir in CheriBSD
    --build-dir .
    --ssh-port $SSHPORT
    --ssh-key $HOME/.ssh/id_ed25519.pub
)
export BUILDBOT_PLATFORM=riscv64-purecap
args=${args[@]}
script -O /dev/null -ec "python3 tests/run_cheri_examples.py $args"

echo "Running tests for 'morello-hybrid' using QEMU..."
args=(
    --architecture morello-hybrid
    # Qemu System to use
    --qemu-cmd $CHERI_DIR/sdk/bin/qemu-system-morello
    --bios edk2-aarch64-code.fd
    --disk-image $CHERI_DIR/cheribsd-morello-hybrid.img
    # Required build-dir in CheriBSD
    --build-dir .
    --ssh-port $SSHPORT
    --ssh-key $HOME/.ssh/id_ed25519.pub
)
export BUILDBOT_PLATFORM=morello-hybrid
args=${args[@]}
script -O /dev/null -ec "python3 tests/run_cheri_examples.py $args"

echo "Running tests for 'morello-purecap' using QEMU..."
args=(
--architecture morello-purecap
# Qemu System to use
--qemu-cmd $CHERI_DIR/sdk/bin/qemu-system-morello
--bios edk2-aarch64-code.fd
--disk-image $CHERI_DIR/cheribsd-morello-purecap.img
# Required build-dir in CheriBSD
--build-dir .
--ssh-port $SSHPORT
--ssh-key $HOME/.ssh/id_ed25519.pub
)
export BUILDBOT_PLATFORM=morello-purecap
args=${args[@]}
script -O /dev/null -ec "python3 tests/run_cheri_examples.py $args"
