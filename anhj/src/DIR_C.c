#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include "../include/msg_data.h"
#define BUFFERSIZE 16

void error_handling(char *message);
int main(int argc, char*argv[])
{

	if(argc!=4)
	{
		fprintf(stderr,"<USAGE> DIR_C <IP> <PORT> <msgcount>\n");
		return 0;
	}
	/****************************/
	/*메세지 큐 세팅	   		*/
	/****************************/
        key_t key=12345;
        int msqid;
        struct message msg;
	/**************************/
	/*socket 통신을 위한 세팅*/
	/*************************/
	int sock;
	struct sockaddr_in serv_addr;
	char message[64];
	char IP[16];
	strcpy(IP,argv[1]);
	int port=atoi(argv[2]);
	int str_len;
	int option;
	int count=atoi(argv[3]); 
	int getsockopt(int s, int level, int optname, void*optval, socklen_t *optlen);
	int setsockopt(int s, int level, int optname, const void *optval, socklen_t optlen);

   	//받아오는 쪽의 msqid얻어오고
    printf("msgget start\n");
    if((msqid=msgget(key,IPC_CREAT|0666))==-1)
	{
        printf("msgget failed\n");
    	exit(0);
    }
	//socket 설정
	sock=socket(PF_INET,SOCK_STREAM,0);
	option=1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&option,sizeof(option));
	if(sock == -1)
		error_handling("socket() error");

	// 구조체 초기화
	memset(&serv_addr, 0,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr("192.168.1.207");
	serv_addr.sin_port=htons(9132);

    //메시지를 받는다.
	int i=0;
	while(i<count)
	{
        	if(msgrcv(msqid,&msg,sizeof(struct real_data),0,0)==-1)
		{
                	printf("msgrcv failed\n");
			continue;
       	}		
	strcpy(message,msg.data.name);
	/************/
	/*소켓 생성 */
	/************/
	//연결 요청
	if(connect(sock, (struct sockaddr*)&serv_addr,sizeof(serv_addr))==1)
		error_handling("connect() error!");
	str_len=write(sock,message,sizeof(message));
	if(str_len==-1)
		error_handling("write() error!");

	memset(message,0x00,sizeof(message));
	i++;
	usleep(1000);
	}
	close(sock);
}
void error_handling(char *message)
{
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}
