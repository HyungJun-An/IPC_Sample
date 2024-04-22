#include <sys/poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>
#include <iconv.h>

int order_count;
char ip_addr[32];

#define BUFFER_SIZE 128

int recv_count;
/********************************/
/* DIR_B 연결                   */
/********************************/
int clientSocket;
struct sockaddr_in DIRBAddress;
/********************************/
/* DIR_M 연결                   */
/********************************/
struct sockaddr_in client_addr;
socklen_t client_addr_size = sizeof(client_addr);
int DIRMSock;

FILE* fp;

int init_DIR_connect (char *);            
int timefile_write (int , int);
int cal_data_write(char *, char *, int, int);
int time_check(char*  , char* , int);
int recv_msg_func(char* );

int main(int argc, char* argv[]) 
{
    pthread_t tid1, tid2;

    memset(ip_addr, 0x00, sizeof(ip_addr));
    memcpy(ip_addr, argv[1], strlen(argv[1]));
    order_count = atoi(argv[2]);
    
    int i;
	char message[128] = "START";
    char recv_msg[BUFFER_SIZE];

    int accm_time;
    int stime, rtime;
    char send_time[512], recv_time[512];


    accm_time = 0;

    /* init */
    init_DIR_connect( ip_addr);
    recv_count = 0;

    for(i = 0 ; i < order_count; i++)
    {
	    /* DIR_B로 데이터 송신 */
        
        if(i+1 == order_count)
        {
            memset(message, 0x00, sizeof(message));
            memcpy(message, "END", strlen("END"));     
        }
        
        memset(send_time, 0x00, sizeof(send_time));
        stime = time_check(send_time, message, i+1);   /* 송신시간을 저장 */

	    if (send(clientSocket, message, sizeof(message), 0) == -1) 
        {
		    printf("MCI_A 메시지 전송 실패\n");
	        close(clientSocket);
            exit(0);
        }

        memset(message, 0x00, sizeof(message));
        memcpy(message, "HELLO TITAN DDS TEST", strlen("HELLO TITAN DDS TEST"));     
        

        printf("MCI_A  SEND [%d/%d]....\n", i+1, order_count);
        memset(recv_msg,0x00, sizeof(recv_msg));
        recv_msg_func(recv_msg);                       /* DIR_M으로 부터 데이터 수신 */
        memset(recv_time, 0x00, sizeof(recv_time));    /* 수신시간을 저장 */
        rtime = time_check(recv_time, recv_msg, i+1);
        printf("MCI_A  RECV [%d/%d]....\n", i+1, order_count);

        /* 시간계산 파일쓰기 */
        /* interval time 누적하기 (평균낼때 사용) */
        accm_time += cal_data_write(send_time, recv_time, stime, rtime);

    }


    /* 최종적으로 파일저장함 */
#if 0
    timefile_write(accm_time, order_count);
#endif

    printf("MCI_A 전송 완료\n");

    fclose(fp);
	close(clientSocket);
    close(DIRMSock);
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
        printf("MCI_A GETTIMEFDAY() CALL ERROR\n");
    }
    if((lt = localtime(&tv.tv_sec)) == NULL)
    {
        printf("MCI_A localtime() call error\n");
    }

    sprintf(time_data, "%d, %s, %02d:%02d:%02d-%06d\n", data_cnt, message,
            lt->tm_hour, lt->tm_min, lt->tm_sec, tv.tv_usec);

    /* cal_data_write()로 평균시간을 내기 위해 시간저장 */
    temp_time = (lt->tm_hour * 3600) + (lt->tm_min * 60)+ lt->tm_sec;
    cal_time = (temp_time * 1000000)+ tv.tv_usec;
    

    return cal_time;
}


int recv_msg_func(char* recv_msg)
{

    int rtn;
    int r;
    int poll_result;
    int    timeout;
    struct pollfd fds[1];
    char buf[BUFFER_SIZE];
    
    // 버퍼 설정

    fds[0].fd = DIRMSock;
    fds[0].events = POLLIN;


    poll (fds, 1, -1);

    // 클라이언트 요청 읽기
    memset(buf, 0x00, sizeof(buf));
    r = read(DIRMSock, buf, BUFFER_SIZE);
    if(memcmp(buf, "CLOSE", 5) == 0)
    {
        printf("MCI_A %s\n", buf);
        return -1;
    }
    if (r == -1)
    {
        printf("MCI_A read error!\n");
        close(DIRMSock);
        return -1;
    }

    if (send(DIRMSock, buf, sizeof(buf), 0) == -1)
        printf("MCI_A 전송 실패\n");

    recv_count++;
    memset(recv_msg, 0x00, sizeof(recv_msg)); 
    memcpy(recv_msg, buf, strlen(buf));

    return 0;
}

int init_DIR_connect (char *ip_addr)
{
    struct sockaddr_in server_addr;
    int server_sock;
    int rtn;

    /****************************************************************************/
    /* FOR TEST FILE WRITE                                                      */
    /****************************************************************************/
    fp = fopen ("Latency_TEST.csv", "w");
    if(fp == NULL)
    {
        printf("MCI_A File Open Error\n");
        exit(0);
    }
    /****************************************************************************/
    /* client(DIR_B) connect Start                                              */
    /****************************************************************************/
	// 소켓 생성
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == -1) 
    {
		printf("MCI_A 소켓 생성 실패");
        exit(0);
	}

	// 서버 정보 설정
	DIRBAddress.sin_family = AF_INET;
	DIRBAddress.sin_port = htons(9999);
	DIRBAddress.sin_addr.s_addr = inet_addr(ip_addr);

    while(1)
    {
	    // 서버에 연결
	    if (connect(clientSocket, (struct sockaddr *)&DIRBAddress, sizeof(DIRBAddress)) == -1) 
        {
		    printf("MCI_A 연결 실패");
            sleep(1);
	    } 
        else 
        {
		    printf("MCI_A 연결 성공\n");
            break;
	    }
    }
    /****************************************************************************/
    /* client connect End                                                       */
    /****************************************************************************/
    /****************************************************************************/
    /* Server(DIR_M) connect Start                                              */
    /****************************************************************************/
    server_sock= socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1)
    {
        printf("MCI_A socket create error!\n");
        return(-1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5555);
    server_addr.sin_addr.s_addr = inet_addr(ip_addr);

    rtn = bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (rtn == -1)
    {
        printf("MCI_A bind error!");
        return(-1);
    }

    rtn = listen(server_sock, 5);
    if (rtn == -1)
    {
        printf("MCI_A listen error!\n");
        return(-1);
    }

    /* 서버 소켓이 클라이언트 연결 요청 수락 */
    DIRMSock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_size);
    if (DIRMSock == -1)
    {
        printf("MCI_A accept error!\n");
        return(-1);
    }

    return 0;

}
