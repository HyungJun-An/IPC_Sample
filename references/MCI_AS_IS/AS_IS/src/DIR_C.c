#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
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
int mq_key; // Message queue ID
int clientSocket;
struct sockaddr_in serverAddress;

int main(int argc, char* argv[]) 
{
    int i;
    int rtn;
    msgbuf msg;
	char message[BUFFER_SIZE];
    char buf[BUFFER_SIZE];

    /***************************/
    /* 초기화                  */
    /***************************/
    if( argc != 2)
    {
        printf("DIR_C Use Error : processname IP Port 'Ex : DIR_B 127.0.0.1' ");
        exit(0);
    }
    rtn = init(argv);
    if(rtn == -1)
    {
        printf("DIR_C [ERROR] INIT \n");
        exit(0);
    }

    while(1)
    {
        if (msgrcv(mq_key, &msg, sizeof(msg), 1, 0) == -1) 
        { 
            printf("DIR_C Message Receiving Finished!\n");
            exit(0);
        }
        
        memset(message, 0x00, sizeof(message));
        memcpy(message, msg.buf, BUFFER_SIZE);
	    // 메시지 전송
	    if (send(clientSocket, message, sizeof(message), 0) == -1) 
        {
		    printf("DIR_C 메시지 전송 실패\n");
	        close(clientSocket);
		    return 1;
        }
         memset(buf, 0x00, sizeof(buf));
        rtn = read(clientSocket, buf, BUFFER_SIZE);
        if (rtn == -1)
        {
            printf("MCI_A  read error!\n");
            close(clientSocket);
            exit(0);
        }

        if(memcmp(message, "END", strlen("END")) == 0)
        {
            sleep(3);
            break; 
        }
    }

	close(clientSocket);
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
    mq_key = msgget((key_t) 3456, IPC_CREAT|0666);
    if (mq_key == -1)
    {
        printf("DIR_C Message Get Failed!\n");
        return -1;
    }

	// 소켓 생성
    /**********************************/
    /* TCP 접속                       */
    /**********************************/
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == -1) 
    {
		printf("DIR_C 소켓 생성 실패");
        return -1;
	}

	// 서버 정보 설정
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(7777);
	serverAddress.sin_addr.s_addr = inet_addr(argv[1]);

    while(1)
    {
	    // 서버에 연결
	    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) 
        {
		    printf("DIR_C 연결 실패");
            sleep(1);
	    } 
        else 
        {
		    printf("DIR_C 연결 성공\n");
            break;
	    } 
    }

    return 0;
}
