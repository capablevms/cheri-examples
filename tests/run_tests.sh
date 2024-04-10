#!/bin/bash
#
# Copyright (c) 2021 The CapableVMs "CHERI Examples" Contributors.
# SPDX-License-Identifier: MIT OR Apache-2.0
#
# Test that examples execute as expected.
#
# This is designed to be run by .buildbot.sh, but can also be run manually too,
# as long as you have a suitable environment, and an execution platform set up
# and running so that the make-based run-* targets have something to connect to.
# Notable environment variables:
#
# - BUILDBOT_PLATFORM
#   - Used to find the example makefile. Set to something like "morello-purecap"
#     or "riscv64-purecap".
# - SSHPORT
# - SSHUSER (defaults to 'root')
# - SSHHOST (defaults to 'localhost')
#
# TODO: Extend this to cover interactive examples and other architectures.

set -e

ERROR=false
if [ -z "$SSHPORT" ]; then
    echo "SSHPORT is empty or not set" >&2
    ERROR=true
fi
if [ -z "$BUILDBOT_PLATFORM" ]; then
    echo "BUILDBOT_PLATFORM is empty or not set" >&2
    ERROR=true
fi
$ERROR && exit 1

export SSH_OPTIONS='-o "StrictHostKeyChecking no"'
export SCP_OPTIONS='-o "StrictHostKeyChecking no"'

function run {
    pushd "$2"
    for name in "${@:3}"; do
        # These tests should trigger an "In-address space security exception"
        # So they should fail, i.e. `exit=1`
        if [ "$1" = "to_fail" ]; then
            exit_status=0
            RESULT={{$(make -f Makefile.$BUILDBOT_PLATFORM run-$name)} && exit_status=1} || true
            if [ $exit_status != 0 ]; then
                exit 1
            fi
        else
            make -f Makefile.$BUILDBOT_PLATFORM run-$name
        fi
    done
    # popd so long as we're not working in the repo root
    if [ "$2" != "." ]; then
        popd
    fi
}

# TODO: `mmap` also fails, but looks like it is intended to pass.

# PURECAP tests
# TODO: add previous examples
if [ "$1" = "riscv64" ] || [ "$1" = "morello-purecap" ]; then
    # Tests that should pass
    run OK shared_objects shared_objects-pcc_bounds_check_main
    run OK capability_sharing/read-cap-from-file read-cap-from-file
    run OK capability_sharing/mmap-shared-vs-private private_anon_main
    run OK capability_sharing/mmap-shared-vs-private private_file_main
    run OK capability_sharing/mmap-shared-vs-private shared_anon_main
    run OK capability_sharing/read-cap-from-pipe main
    run OK capability_sharing/read-cap-from-pipe-with-sh read-cap-from-pipe-with-sh
    # Tests that should fail
    run to_fail capability_sharing/cap-to-file cap_to_file
    run to_fail capability_sharing/leak-capability leak-capability
    run to_fail capability_sharing/mmap-shared-vs-private shared_file_main
    run to_fail compare_platforms compare_platforms_overflow
    run to_fail compare_platforms compare_platforms_read_only
elif [ "$1" = "morello-hybrid" ]; then
    # HYBRID TESTS
    # Tests that should fail
    run to_fail hybrid/ddc_compartment_switching ddc_compartment_switching_nok
    run to_fail hybrid ddc_invalid ddc_null
    run to_fail hybrid/compartment_examples/inter_comp_call/malicious_compartments inter_comp_call-secure-try_deref
    run to_fail hybrid/compartment_examples/inter_comp_call/malicious_compartments inter_comp_call-secure-redirect_clr
    run to_fail hybrid/compartment_examples/inter_comp_call/malicious_compartments inter_comp_call-secure-update_ddc
    # Tests that should pass
    run OK compare_platforms compare_platforms_overflow
    run OK example_allocators/compartment_alloc main
    run OK hybrid basic_ddc
    run OK hybrid cap_build
    run OK hybrid/compartment_examples/inter_comp_call/base main
    run OK hybrid/compartment_examples/inter_comp_call/malicious_compartments inter_comp_call-secure
    run OK hybrid/ddc_compartment_switching ddc_compartment_switching
    run OK syscall-restrict syscall-restrict
else
    echo "$1 not recognised."
    exit 1
fi

# TODO: 'timsort' works, but takes a very long time. Is it useful to test a
# smaller data set?
