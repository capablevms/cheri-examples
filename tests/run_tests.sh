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
# For now, we just test examples on `morello-hybrid`.

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
    popd
}

# TODO: `mmap` also fails, but looks like it is intended to pass.

# TODO: PURECAP tests

# HYBRID TESTS
# Tests that should fail
run to_fail hybrid/ddc_compartment_switching ddc_compartment_switching_nok
run to_fail hybrid ddc_invalid ddc_null
run to_fail hybrid/compartment_examples/inter_comp_call/secure-try_deref main
# Tests that should pass
run OK hybrid/ddc_compartment_switching ddc_compartment_switching
run OK hybrid basic_ddc
run OK hybrid/compartment_examples/inter_comp_call/base main
run OK hybrid/compartment_examples/inter_comp_call/secure main

# TODO: 'timsort' works, but takes a very long time. Is it useful to test a
# smaller data set?
