#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/poll.h>
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
int mq_key; // Message queue ID
struct sockaddr_in server_addr;
int server_sock; 

/***************************/
/* 함수선언                */
/***************************/
int init(char* argv[]);                 /* 초기화 함수       */
int mq_write(char *buf, int cnt);       /* MessageQueue 전송 */

/********************************************************************************/
/* Process Name  : FEP_G                                                        */
/* Process 설명  : TCP 수신 데이터 MQ로 전송 프로세스                           */
/* Date : 2024.3.26.                                                            */
/********************************************************************************/
int main(int argc, char *argv[])
{
	int r;
    int rtn ;
    int i;
    int poll_result;
	char buf[BUFFER_SIZE];
    int    timeout;
    struct pollfd fds[1];
	// 클라이언트 주소 구조체 설정
	struct sockaddr_in client_addr;
	socklen_t client_addr_size = sizeof(client_addr);
   
    /***************************/
    /* 초기화                  */
    /***************************/
    if( argc != 2)
    {
        printf("FEP_G Use Error : processname IP Port 'Ex : DIR_B 127.0.0.1 ' ");
        exit(0);
    }
    rtn = init(argv);
    if(rtn == -1)
    {
        printf("FEP_G [ERROR] INIT \n"); 
    }

	// 서버 소켓이 클라이언트 연결 요청 수락
	int client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_size);
	if (client_sock == -1)
	{
		printf("FEP_G accept error!\n");
		return 0;
	}
	
	// 버퍼 설정

    fds[0].fd = client_sock;
    fds[0].events = POLLIN;
    timeout = (3* 60 * 1000);

    int data_cnt = 0;

    while(1)
    {
        poll_result = poll (fds, 1, timeout);

        if( poll_result == -1)
        {
            printf("FEP_G poll error \n");
            break;
        }
        if( poll_result == 0) 
        {
            printf("FEP_G poll timeout \n");
            continue;        /* timeout */
        }

	    // 클라이언트 요청 읽기
	    r = read(client_sock, buf, BUFFER_SIZE);
	    if (r == -1)
	    {
		    printf("FEP_G read error!\n");
	        close(client_sock);
		    exit(0);
	    }


        data_cnt++;
        /* MessageQueue 전송 */
        mq_write(buf, data_cnt);
	    /* 클라이언트에게 동일한 메세지를 전송 */
        if(memcmp(buf, "END", strlen("END")) == 0)
        {
            break;
        }
	    if (send(client_sock, buf, sizeof(buf), 0) == -1) 
            printf("FEP_G 전송 실패\n");

	}
	close(client_sock);

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
    mq_key = msgget((key_t) 2345, IPC_CREAT|0666);
    if (mq_key == -1) 
    {
        printf("FEP_G Message Get Failed!\n");
        exit(0);
    }

    /**********************************/
    /* Socket 초기화                  */
    /**********************************/
    server_sock= socket(AF_INET, SOCK_STREAM, 0);
	if (server_sock == -1)
	{
		printf("FEP_G socket create error!\n");
		return 0;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(7777);
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);

	rtn = bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (rtn == -1)
	{
		printf("FEP_G bind error!");
		return rtn;
	}
	
	rtn = listen(server_sock, 5);
	if (rtn == -1)
	{
		printf("FEP_G listen error!\n");
		return rtn;
	}
    /**********************************/
    /* Socket 초기화 완료             */
    /**********************************/


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
    if(msgsnd(mq_key, (void *)&msg, sizeof(msgbuf), IPC_NOWAIT) == -1)
    {
            perror("mq_send error");
            return(-1);
    }

	return 0;
}
