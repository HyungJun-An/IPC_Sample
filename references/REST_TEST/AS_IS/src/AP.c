#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define BUFFER_SIZE 128
#define  FIFO_FILE "fifo_test"

/****************************/
/* Message buffer structure */
/****************************/
typedef struct {
  long msgtype;          /* Message type, must be > 0 with 'long' data type */
  int value;
  char buf[BUFFER_SIZE]; /* Message data to push in queue */
} msgbuf;

int fd;

/***************************/
/* 전역변수                */
/***************************/
int mq_key; // Message queue ID

int main(int argc, char* argv[]) 
{
    int i;
    int rtn;
    msgbuf msg;
	char message[BUFFER_SIZE];

    /***************************/
    /* 초기화                  */
    /***************************/
    rtn = init(argv);
    if(rtn == -1)
    {
        printf("[ERROR] INIT \n");
        exit(0);
    }

    while(1)
    {
        if (msgrcv(mq_key, &msg, sizeof(msg), 1, 0) == -1) 
        { 
            printf("Message Receiving Finished!\n");
            exit(0);
        }
        
        memset(message, 0x00, sizeof(message));
        memcpy(message, msg.buf, BUFFER_SIZE);
	    // 메시지 전송
        write( fd, message, strlen(message));
        if(memcmp(message, "END", strlen("END")) == 0)
        {
            sleep(3);
            break; 
        }
    }

	close(fd);
	return 0;
}

/*****************************************************/
/* 초기화 함수                                       */
/*****************************************************/
int init(char* argv[] )
{
    int rtn =0;
    /**********************************/
    /* MessageQueue 연결              */
    /**********************************/
    mq_key = msgget((key_t) 6359, IPC_CREAT|0666);
    if (mq_key == -1)
    {
        printf("Message Get Failed!\n");
        return -1;
    }

    fd = open( FIFO_FILE, O_WRONLY);
    if ( fd == -1)
    {
      printf( "fifo open Error\n");
      exit(0);
    }

    return 0;
}
