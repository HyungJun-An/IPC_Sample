#!/bin/sh
if [ $# -lt 1 ]; then
    echo "test_latency.sh order_count ex) test_latency.sh 1000"
    exit 0
fi

DIR_B 127.0.0.1 &
DIR_C 127.0.0.1 &
DIR_L 127.0.0.1 &
DIR_M 127.0.0.1 &
FEP_G 127.0.0.1 &
FEP_H 127.0.0.1 &
FEP_I 127.0.0.1 &
FEP_J 127.0.0.1 &
FEP_K 127.0.0.1 &

MCI_A 127.0.0.1 $1
#MCI_test 127.0.0.1 $1     # ipc에 데이터가 밀려서 처리 안되면 이걸로 쓰세요  친절한 주석 감사합니다ㅎㅎ
