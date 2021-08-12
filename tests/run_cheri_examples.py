#!/usr/bin/env python3
# This script starts a `qemu-system` to run the tests and then safely shutdown
# the VM.
#
# Inspired by https://github.com/CTSRD-CHERI/cheribuild/tree/master/test-scripts

import argparse
import os

from run_tests_common import boot_cheribsd, run_tests_main


def run_cheri_examples_tests(qemu: boot_cheribsd.QemuCheriBSDInstance, args: argparse.Namespace) -> bool:
    if args.sysroot_dir is not None:
        boot_cheribsd.set_ld_library_path_with_sysroot(qemu)
    boot_cheribsd.info("Running tests for cheri-examples")
    
    # This is the BUILD_DIR for the tests, not for this script
    os.system("BUILD_DIR=bin ./tests/run_tests.sh")
    return True

if __name__ == '__main__':
    run_tests_main(test_function=run_cheri_examples_tests, need_ssh=True, should_mount_builddir=False)