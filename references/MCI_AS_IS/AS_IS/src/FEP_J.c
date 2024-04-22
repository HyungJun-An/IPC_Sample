#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define BUFFER_SIZE 128

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
int main(int argc, char* argv[]) 
{
    int i;
    int rtn;
    msgbuf msg;

    /***************************/
    /* 초기화                  */
    /***************************/
    rtn = init();
    if(rtn == -1)
    {
        printf("FEP_J [ERROR] INIT \n");
        exit(0);
    }

    while(1)
    {
        if (msgrcv(mq_recv_key, &msg, sizeof(msg), 1, 0) == -1) 
        { 
            printf("FEP_J Message Receiving Finished!\n");
            exit(0);
        }
        
        if(msgsnd(mq_send_key, (void *)&msg, sizeof(msgbuf), IPC_NOWAIT) == -1)
        {
            perror("mq_send error");
            exit(0);
        }

        if(memcmp(msg.buf, "END", strlen("END")) == 0)
        {
            break;
        }


	}

	return 0;
}

/*****************************************************/
/* 초기화 함수                                       */
/*****************************************************/
int init(void )
{
    /**********************************/
    /* 수신 MessageQueue 연결         */
    /**********************************/
    mq_recv_key = msgget((key_t) 5678, IPC_CREAT|0666);
    if (mq_recv_key == -1)
    {
        printf("FEP_J Message Get Failed!\n");
        return -1;
    }


    /**********************************/
    /* 송신 MessageQueue 연결         */
    /**********************************/
    mq_send_key = msgget((key_t) 6789, IPC_CREAT|0666);
    if (mq_send_key == -1)
    {
        printf("FEP_J Message Get Failed!\n");
        return -1;
    }
    return 0;
}
