#!/bin/sh
if [ $# -lt 1 ]; then
    echo "test2_latency.sh order_count ex) test2_latency.sh 1000"
    exit 0
fi

DIR_C 127.0.0.1 &
DIR_L 127.0.0.1 &
FEP_G 127.0.0.1 &
FEP_H &
FEP_I &
FEP_J &
FEP_K 127.0.0.1 &

DIR_Btest $1
