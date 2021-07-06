#!/bin/sh
# This is a simple way to test "bounds.c" by checking ``stderr``.
set -e

: "${SSHPORT:=10021}"
if [ -f "../bin/bounds" ]; then
    scp -P $SSHPORT ../bin/bounds root@127.0.0.1:/root
    exit_status=0
    RESULT={{$(ssh -p $SSHPORT -t root@127.0.0.1 'sh -c "/root/bounds 2>&1 > /dev/null"')} && exit_status=1} || true
    echo -n "bounds... "
    if [ $exit_status != 0 ]; then
        echo "FAILED! See below for more details."
        echo $RESULT
    else
        echo "ok"
    fi
else
    echo "Please, build first bounds.c before running the tests."
    exit 1
fi
