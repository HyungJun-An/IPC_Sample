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
#define  FIFO_FILE  "fifo_test"
int fifo_fd;

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
int mq_send_key; // Message queue ID

int init(void );
int mq_write(char *, int );
int cal_data_write(int, int);
int time_check(void);


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
    char buff[BUFFER_SIZE];
    char message[BUFFER_SIZE] = "START";

    order_count = atoi(argv[1]);

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
        stime = time_check();   /* 송신시간을 저장 */

        mq_write(message, i);  /* data 송신 */
        printf(" SEND [%d/%d]....\n", i+1, order_count);

        /*************************/
        /* data recv             */
        /*************************/
        memset( buff, 0, BUFFER_SIZE);
        read( fifo_fd, buff, BUFFER_SIZE);

        memset(message, 0x00, sizeof(message));
        memcpy(message, "HELLO AS-IS TEST", strlen("HELLO TITAN DDS TEST"));


        memset(recv_time, 0x00, sizeof(recv_time));    /* 수신시간을 저장 */
        rtime = time_check();
        printf(" RECV [%d/%d]....\n", i+1, order_count);

        /* 시간계산 파일쓰기 */
        /* interval time 누적하기 (평균낼때 사용) */
        accm_time += cal_data_write(stime, rtime);

	}

    printf("전송 완료\n");

    fclose(fp);
    close(fifo_fd);

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
/* 1. 송수신 내용 저장                                                         */
/* 2. 송수신이 걸린 시간 저장                                                  */
/*******************************************************************************/
int cal_data_write(int stime, int rtime)
{
    fprintf(fp, "%d\n", rtime - stime);

    return 0;
}

/*******************************************************************************/
/* 시간 저장 함수                                                              */
/*******************************************************************************/
int time_check(void)
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
    int rt= 0;
    /****************************************************************************/
    /* FOR TEST FILE WRITE                                                      */
    /****************************************************************************/
    fp = fopen ("ASIS_TEST.csv", "w");
    if(fp == NULL)
    {
        printf("File Open Error\n");
        exit(0);
    }

    /**********************************/
    /* FIFO 연결                      */
    /**********************************/
    rt == mkfifo(FIFO_FILE, 0666);
    if (rt == -1 )
    {
      printf( "mkfifo Error\n");
      exit( 0);
    }
    fifo_fd = open(FIFO_FILE, O_RDWR);
    if ( fifo_fd == -1)
    {  
      printf( "fifo open Error\n");
      exit( 0);
    }


    /**********************************/
    /* 송신 MessageQueue 연결         */
    /**********************************/
    mq_send_key = msgget((key_t) 6359, IPC_CREAT|0666);
    if (mq_send_key == -1)
    {
        printf("Message Get Failed!\n");
        return -1;
    }
    return 0;
}
