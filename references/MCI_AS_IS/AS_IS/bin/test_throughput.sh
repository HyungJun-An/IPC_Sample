#!/bin/sh
if [ $# -lt 1 ]; then
    echo "test_throughput.sh order_count ex) test_throughput.sh 1000"
    exit 0
fi

DIR_B 127.0.0.1 &
DIR_C 127.0.0.1 &
FEP_H &
FEP_Itest &
FEP_G 127.0.0.1 &

#MCI_Through 127.0.0.1 $1
