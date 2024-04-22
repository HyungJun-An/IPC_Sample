#include <sys/poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>


int order_count;
char ip_addr[32];

#define BUFFER_SIZE 128

int main(int argc, char* argv[]) 
{
    memset(ip_addr, 0x00, sizeof(ip_addr));
    memcpy(ip_addr, argv[1], strlen(argv[1]));
    order_count = atoi(argv[2]);
     
    int i;
    int r;
    int clientSocket;
	struct sockaddr_in serverAddress;
	char message[BUFFER_SIZE] = "START";
	char buf[BUFFER_SIZE];

    time_t t;
    struct tm *lt;
    struct timeval tv;

	// 소켓 생성
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == -1) 
    {
		printf("MCI 소켓 생성 실패");
        exit(0);
	}

	// 서버 정보 설정
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(9999);
	serverAddress.sin_addr.s_addr = inet_addr(ip_addr);

	// 서버에 연결
	if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) 
    {
		printf("MCI 연결 실패");
        exit(0);
	} 
    else 
    {
		printf("MCI 연결 성공\n");
	}

    for(i = 0 ; i < order_count; i++)
    {
        if(i+1 == order_count)
        {
            memset(message, 0x00, sizeof(message));
            memcpy(message, "END", strlen("END"));
            printf("MCI End [%s]\n", message);
        }

	    // 메시지 전송
	    if (send(clientSocket, message, sizeof(message), 0) == -1) 
        {
		    printf("MCI 메시지 전송 실패\n");
	        close(clientSocket);
            exit(0);
        }
        printf("MCI  SEND [%d/%d]....\n", i+1, order_count);
        memset(buf, 0x00, sizeof(buf));
        r = read(clientSocket, buf, BUFFER_SIZE);
        if (r == -1)
        {
            printf("MCI_A  read error!\n");
	        close(clientSocket);
            exit(0);
        }

        memset(message, 0x00, sizeof(message));
        memcpy(message, "HELLO TITAN DDS THROUGHPUT TEST", strlen("HELLO TITAN DDS THROUGHPUT TEST"));

	}
    sleep(10);
    printf("MCI 종료\n");


	close(clientSocket);
    return 0;  
}

