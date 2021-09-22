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
# TODO: Extend this to cover interactive examples, or those that are expected to
# fail. For now, we just test examples that run, unattended, to completion.

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
    pushd "$1"
    for name in "${@:2}"; do
        make -f Makefile.$BUILDBOT_PLATFORM run-$name
    done
    popd
}

# TODO: `mmap` also fails, but looks like it is intended to pass.
run .         check_length check_mask seal setjmp stackscan

# TODO: 'timsort' works, but takes a very long time. Is it useful to test a
# smaller data set?
