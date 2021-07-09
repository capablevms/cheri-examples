#!/bin/sh
# This is a simple way to test "set_bounds.c" by checking ``stderr``.
set -e

# TODO: CONSIDER REFACTORING ALL THE TESTS IN A SINGLE TEST with a loop
# to do: run-{example}

: "${SSHPORT:=10021}"
ARCH=$(ssh -p $SSHPORT -o LogLevel=QUIET -t root@127.0.0.1 'sh -c "uname -m"')
if ! [ -z "${ARCH##*riscv*}" ]; then
    echo "You can run this test only on riscv64."
    exit 1
fi
if [ -f "../bin/set_bounds" ]; then
    scp -P $SSHPORT ../bin/set_bounds root@127.0.0.1:/root
    exit_status=0
    RESULT={{$(ssh -p $SSHPORT -t root@127.0.0.1 'sh -c "/root/set_bounds 65"')} && exit_status=1} || true
    echo -n "set_bounds... "
    if [ $exit_status != 0 ]; then
        echo "FAILED! See below for more details."
        echo $RESULT | tr -d "{}"
    else
        echo "ok"
    fi
else
    echo "Please, build first set_bounds.c before running the tests."
    exit 1
fi
