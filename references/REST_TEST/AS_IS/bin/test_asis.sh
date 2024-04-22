#!/bin/sh
if [ $# -lt 1 ]; then
    echo "test_asis.sh order_count ex) test_asis.sh 1000"
    exit 0
fi

AP &

CH $1
