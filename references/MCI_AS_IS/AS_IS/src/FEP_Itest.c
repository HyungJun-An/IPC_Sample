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
int time_check(char* , char* );
int cal_data_write(char *, char *, int , int , int );


int main(int argc, char* argv[]) 
{
    int data_cnt;
    int rtn;
    msgbuf msg;
    char start_buf[512], end_buf[512];
    int start_time, end_time;

    /***************************/
    /* 초기화                  */
    /***************************/
    rtn = init();
    if(rtn == -1)
    {
        printf("[ERROR] INIT \n");
        exit(0);
    }

    data_cnt = 0;
    while(1)
    {
        if (msgrcv(mq_recv_key, &msg, sizeof(msg), 1, 0) == -1) 
        { 
            printf("Message Receiving Finished!\n");
            exit(0);
        }
        
        if(data_cnt == 0)
        {
            memset(start_buf, 0x00, sizeof(start_buf));
            start_time = time_check(start_buf , msg.buf);
        }
        
        data_cnt++;

        if(memcmp(msg.buf, "END", strlen("END")) == 0)
        {
            memset(end_buf, 0x00, sizeof(end_buf));
            end_time = time_check(end_buf , msg.buf);
            cal_data_write( start_buf, end_buf, start_time, end_time, data_cnt);

            break;
        }


	}

    fclose(fp);

	return 0;
}

/*******************************************************************************/
/* 시간 저장 함수                                                              */
/*******************************************************************************/
int time_check(char* time_data , char* message)
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

    sprintf(time_data, "%s, %02d:%02d:%02d-%06d\n", message,
            lt->tm_hour, lt->tm_min, lt->tm_sec, tv.tv_usec);

    /* cal_data_write()로 평균시간을 내기 위해 시간저장 */
    temp_time = (lt->tm_hour * 3600) + (lt->tm_min * 60)+ lt->tm_sec;
    cal_time = (temp_time * 1000000)+ tv.tv_usec;


    return cal_time;
}


int cal_data_write(char *send_time, char *recv_time, int stime, int rtime, int order_count)
{
    int accmval ;
#if 0
    fprintf(fp, "최초수신데이터 ,  %s", send_time);
    fprintf(fp, "최종수신데이터 ,  %s", recv_time);
    accmval = rtime - stime;  /* 누적시간 */
    fprintf(fp, "누적시간, %d, 초당건수 %d, 평균시간,  %d,  주문건수,  %d\n",
             accmval, 1000000/(accmval/order_count), accmval/order_count, order_count);
#endif
    accmval = rtime - stime;  /* 누적시간 */
    fprintf(fp, "1sec/cnt, %d, average, %d, order count,  %d\n",
              1000000/(accmval/order_count), accmval/order_count,order_count);


    return 0;
}


/*****************************************************/
/* 초기화 함수                                       */
/*****************************************************/
int init(void )
{
    /****************************************************************************/
    /* FOR TEST FILE WRITE                                                      */
    /****************************************************************************/
    fp = fopen ("Throughput_TEST.csv", "w");
    if(fp == NULL)
    {
        printf("File Open Error\n");
        exit(0);
    }


    /**********************************/
    /* 수신 MessageQueue 연결         */
    /**********************************/
    mq_recv_key = msgget((key_t) 4567, IPC_CREAT|0666);
    if (mq_recv_key == -1)
    {
        printf("Message Get Failed!\n");
        return -1;
    }


    /**********************************/
    /* 송신 MessageQueue 연결         */
    /**********************************/
    mq_send_key = msgget((key_t) 5678, IPC_CREAT|0666);
    if (mq_send_key == -1)
    {
        printf("Message Get Failed!\n");
        return -1;
    }
    return 0;
}
