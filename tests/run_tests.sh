#!/bin/sh
# Test runner to test the following examples:
# bounds.c, set_bounds.c, general_bounds.c, xor_pointers.c,
# read_only.c (from the employee example)
#
# *** NOTE ***
# The following examples do not need to be
# tested because they are simply exploring features or
# they are tests themselves/experiments:
# allocate.c, check_length.c, check_mask.c, function.c,
# seal.c, sentry.c, setjmp.c, stackscan.c
set -e

: "${BUILD_DIR:="../bin"}"
: "${SSHPORT:=10021}"
# Examples that must trigger an "In-address space security exception"
EXAMPLES="bounds set_bounds general_bounds xor_pointers read_only"
EMPLOYEE_DIR="./employee/bin"
ARCH=$(ssh -o "StrictHostKeyChecking no" -p $SSHPORT -t root@127.0.0.1 uname -m)

if  [ ! $(find $EMPLOYEE_DIR -prune -empty 2>/dev/null) ]; then
    cp $EMPLOYEE_DIR/read_only $BUILD_DIR/read_only
else
    echo "WARNING: you have not built the employee example yet!"
    echo "SKIPPING employee/read_only..."
fi

for example in ${EXAMPLES}; do
    if  [ ! $(find $BUILD_DIR -prune -empty 2>/dev/null) ]; then
        if [ ! -z "${ARCH##*riscv*}" ] && [ "$example" = "set_bounds" ]; then
            echo "Skipping '$example' because you are not on riscv64..."
            continue
        fi
        scp -o "StrictHostKeyChecking no" -P $SSHPORT "$BUILD_DIR/$example" root@127.0.0.1:/root
        exit_status=0
        RESULT={{$(ssh -o "StrictHostKeyChecking no" -p $SSHPORT -t root@127.0.0.1 "/root/$example 68")} && exit_status=1} || true
        if [ "$example" = "read_only" ]; then
            example=employee/$example
        fi
        echo -n ""$example"... "
        if [ $exit_status != 0 ]; then
            echo "FAILED! See below for more details."
            echo $RESULT | tr -d "{}"
        else
            echo "ok"
        fi
    else
        echo "Please, build first the examples before running the tests."
        exit 1
    fi
done