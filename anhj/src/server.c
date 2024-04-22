#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sock;
	int clnt_sock;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	char message[]="Hello World!";
	
	if(argc!=2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	int getsockopt(int s, int level, int optname, void*optval,socklen_t *optlen);
	int setsockopt(int s, int level, int optname, const void *optval, socklen_t optlen);
	int option;
	serv_sock=socket(PF_INET, SOCK_STREAM, 0); // 1. 소켓 생성
	if(serv_sock == -1)
		error_handling("socket() error");
	option=1;
	setsockopt(serv_sock,SOL_SOCKET,SO_REUSEADDR,&option,sizeof(option));
    //2. 구조체(주소) 초기화
	memset(&serv_addr, 0, sizeof(serv_addr));
    
    //IP 주소와 PORT번호 할당
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(atoi(argv[1]));
	
    // 3. 주소 정보 할당
	if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1 )
		error_handling("bind() error"); 
	
    // 연결요청 가능한 상태로 변경
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");
	
	clnt_addr_size=sizeof(clnt_addr); 
    
    //4. 연결요청에 대한 수락
	clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr,&clnt_addr_size);
	if(clnt_sock==-1)
		error_handling("accept() error");  
	
	write(clnt_sock, message, sizeof(message));
    
    // 5. 소켓 소멸
	close(clnt_sock);	
	close(serv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
