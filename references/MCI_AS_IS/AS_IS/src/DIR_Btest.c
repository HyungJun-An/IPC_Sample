#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/msg.h>

#define BUFFER_SIZE 128
FILE* fp;

/****************************/
/* Message buffer structure */
/****************************/
typedef struct {
  long msgtype;          /* Message type, must be > 0 with 'long' data type */
  int value;
  char buf[BUFFER_SIZE]; /* Message data to push in queue */
} msgbuf;

/***************************/
/* 전역변수                */
/***************************/
int mq_recv_key; // Message queue ID
int mq_send_key; // Message queue ID

int init(void );
int mq_write(char *, int );
int timefile_write (int , int);
int cal_data_write(char *, char *, int, int);
int time_check(char*  , char* , int);


FILE* fp;

int main(int argc, char* argv[]) 
{
    int i;
    int order_count;
    int rtn;
    msgbuf msg;

    int accm_time;
    int stime, rtime;
    char send_time[512], recv_time[512];

    order_count = atoi(argv[1]);
    char message[128] = "START";

    /***************************/
    /* 초기화                  */
    /***************************/
    rtn = init();
    if(rtn == -1)
    {
        printf("[ERROR] INIT \n");
        exit(0);
    }

    accm_time = 0;

    for(i = 0 ; i < order_count; i++)
    {
        if(i+1 == order_count)
        {
            memset(message, 0x00, sizeof(message));
            memcpy(message, "END", strlen("END"));
        }

        memset(send_time, 0x00, sizeof(send_time));
        stime = time_check(send_time, message, i+1);   /* 송신시간을 저장 */

        mq_write(message, i);  /* data 송신 */
        printf(" SEND [%d/%d]....\n", i+1, order_count);

        if (msgrcv(mq_recv_key, &msg, sizeof(msg), 1, 0) == -1)  /* data 수신 */
        { 
            printf("Message Receiving Finished!\n");
            exit(0);
        }
        memset(message, 0x00, sizeof(message));
        memcpy(message, "HELLO TITAN DDS TEST2", strlen("HELLO TITAN DDS TEST"));


        memset(recv_time, 0x00, sizeof(recv_time));    /* 수신시간을 저장 */
        rtime = time_check(recv_time, msg.buf, i+1);
        printf(" RECV [%d/%d]....\n", i+1, order_count);

        /* 시간계산 파일쓰기 */
        /* interval time 누적하기 (평균낼때 사용) */
        accm_time += cal_data_write(send_time, recv_time, stime, rtime);

	}

#if 0
    /* 최종적으로 파일저장함 */
    timefile_write(accm_time, order_count);
#endif

    printf("전송 완료\n");

    fclose(fp);

	return 0;
}

/*****************************************************/
/* MessageQueue 송신 함수                            */
/*****************************************************/
int mq_write(char *buf, int cnt)
{
    msgbuf msg;
    /**************************************************/
    /* MQ 입력                                        */
    /**************************************************/
    msg.msgtype = 1;
    msg.value = cnt;
    memcpy(msg.buf, buf, BUFFER_SIZE);

    if(msgsnd(mq_send_key, (void *)&msg, sizeof(msgbuf), IPC_NOWAIT) == -1)
    {
            perror("mq_send error");
            return(-1);
    }

    return 0;
}

/*******************************************************************************/
/* 평균시간, 주문건수, 초당건수 파일로 저장                                    */
/************************************************* *****************************/
int timefile_write (int accm_time, int order_count)
{
    fprintf(fp, "누적시간, %d, 초당건수 %d, 평균시간,  %d,  주문건수,  %d\n",
             accm_time, 1000000/(accm_time/order_count), accm_time/order_count, order_count);
    return 0;
}


/*******************************************************************************/
/* 1. 송수신 내용 저장                                                         */
/* 2. 송수신이 걸린 시간 저장                                                  */
/*******************************************************************************/
int cal_data_write(char *send_time, char *recv_time, int stime, int rtime)
{
    int interval ;
#if 0
    fprintf(fp, "송신데이터 ,  %s", send_time);
    fprintf(fp, "수신데이터 ,  %s", recv_time);
    fprintf(fp, "소요시간,  %d\n", rtime - stime);
#endif
    fprintf(fp, "%d\n", rtime - stime);
    interval = rtime - stime;  /* 수신시간 - 송신시간 = 송수신 걸린시간 */

    return interval;
}

/*******************************************************************************/
/* 시간 저장 함수                                                              */
/*******************************************************************************/
int time_check(char* time_data , char* message, int data_cnt)
{
    time_t t;
    struct tm *lt;
    struct timeval tv;
    int cal_time;
    int temp_time;

    if((t = gettimeofday(&tv, NULL)) == -1)
    {
        printf("GETTIMEFDAY() CALL ERROR\n");
    }
    if((lt = localtime(&tv.tv_sec)) == NULL)
    {
        printf("localtime() call error\n");
    }

    sprintf(time_data, "%d, %s, %02d:%02d:%02d-%06d\n", data_cnt, message,
            lt->tm_hour, lt->tm_min, lt->tm_sec, tv.tv_usec);

    /* cal_data_write()로 평균시간을 내기 위해 시간저장 */
    temp_time = (lt->tm_hour * 3600) + (lt->tm_min * 60)+ lt->tm_sec;
    cal_time = (temp_time * 1000000)+ tv.tv_usec;


    return cal_time;
}


/*****************************************************/
/* 초기화 함수                                       */
/*****************************************************/
int init(void )
{
    /****************************************************************************/
    /* FOR TEST FILE WRITE                                                      */
    /****************************************************************************/
    fp = fopen ("Latency_TEST2.csv", "w");
    if(fp == NULL)
    {
        printf("File Open Error\n");
        exit(0);
    }


    /**********************************/
    /* 수신 MessageQueue 연결         */
    /**********************************/
    mq_recv_key = msgget((key_t) 7890, IPC_CREAT|0666);
    if (mq_recv_key == -1)
    {
        printf("Message Get Failed!\n");
        return -1;
    }


    /**********************************/
    /* 송신 MessageQueue 연결         */
    /**********************************/
    mq_send_key = msgget((key_t) 3456, IPC_CREAT|0666);
    if (mq_send_key == -1)
    {
        printf("Message Get Failed!\n");
        return -1;
    }
    return 0;
}
