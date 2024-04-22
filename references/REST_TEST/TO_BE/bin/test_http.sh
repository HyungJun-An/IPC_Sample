#!/bin/sh
if [ $# -lt 1 ]; then
    echo "test_http.sh order_count ex) test_http.sh 1000"
    exit 0
fi

server 2222 &
client $1
