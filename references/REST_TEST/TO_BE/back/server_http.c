#include <stdio.h>
#include <arpa/inet.h>  /* GET IP ADDRESS IN 64 BIT MACHINE */
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BACKLOG 10
#define BUF_SIZE 1024
#define FILE_NAME 100

/* FILE HEADER( 출력 결과별html형식의 header ) */

char imageheader[] = "HTTP/1.1 200 ok\r\n" "Content-Type: image/jpeg\r\n\r\n";
char gifheader[] = "HTTP/1.1 200 ok\r\n" "Content-Type: image/gif\r\n\r\n";
char mp3header[] = "HTTP/1.1 200 ok\r\n" "Content-Type: audio/mp3\r\n\r\n";
char pdfheader[] = "HTTP/1.1 200 ok\r\n" "Content-Type: application/pdf\r\n\r\n";
char htmlheader[] = "HTTP/1.1 200 ok\r\n" "Content-Type: text/html\r\n\r\n";
char defaulthtml[] = "HTTP/1.1 4040 Not Found\r\n" 
                     "Content-Type: text/html; charset=UTF-8\r\n\r\n"
                     "<!DOCTYPE html>\r\n" "<html><head><title>WELCOM TO MY SERVER</title>\r\n"
                     "<body>I've got wrong file name. check it please.</body>"
                     "</html>";

int main(int argc, char *argv[])
{
    int sockfd, new_fd;
    struct sockaddr_in my_addr;       /* server addr */
    struct sockaddr_in their_addr;    /* client addr */
    char buffer[BUF_SIZE];
    char *strptr1;
    int count = 0;
    int n;                            /* reading variable */
    int sin_size;                     /* client addr size */
    int filesize;
    char filename[FILE_NAME];
    char pwd[BUF_SIZE];
    int rt;
    char fbuf[BUF_SIZE];

    int i = 0;
    int fd;

    /* 포트번호 파라미터로 입력 */
    int PORT = atoi(argv[1]);
    if(argc<2)
    {
        printf("[ERROR] NO PORT\n");
        exit(0);
    }

    /**********************************/
    /* Socket 초기화                  */
    /**********************************/
    sockfd= socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket create error!\n");
        exit(0);
    }

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(PORT);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    rt = bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr));
    if (rt == -1)
    {
        perror("bind error!");
        exit(0);
    }

    rt = listen(sockfd, BACKLOG);
    if (rt == -1)
    {
        perror("listen error!\n");
        exit (0);
    }

    sin_size = sizeof(struct sockaddr_in);

    while(1)
    {
        /* 접속 대기 */
        if((new_fd=accept(sockfd, (struct sockaddr*)&their_addr, &sin_size))==-1)
        {
            perror("fail to accepting");
            continue;
        }

        memset(buffer, 0x00, BUF_SIZE);
        n=read(new_fd, buffer, BUF_SIZE);
        if(n<0)
        {
            perror("reading error");
        }

        /* Find FileName */
        strptr1 = strstr(buffer, "GET /");      
        strptr1 +=5;
  
        memset(filename, 0x00, FILE_NAME);
        i = 0;
        while(1)
        {
            if(strptr1[i] ==  0x20 )          /* 내용의끝에 스페이스 치환(\0)후 종료 */
            {
                filename[i] = '\0'; break;
            }
            filename[i]=strptr1[i];
            i++;
            if(i > FILE_NAME)
            {
                printf("filename overwrite [%s]\n" , strptr1);
                break;
            }
        }

        write(new_fd, pdfheader, sizeof(pdfheader)-1); /* header */
        memset(fbuf, 0x00, sizeof(fbuf));
        memcpy(fbuf, "SUCCESS", strlen("SUCCESS"));
        write(new_fd, fbuf, strlen(fbuf));

#if 0
        /* 현재 작업중인 디렉터리내 파일 검색 */
        getcwd(pwd, BUF_SIZE);   /* 현재의 디렉터리 경로 */
        strcat(pwd,"/");         /* 현재의 경로에 /을 붙여준다 */
        strcat(pwd,filename);    /* 
        /* 파일을 찾을 수 없음 */
        fd=open(pwd, O_RDONLY);
       
        printf("pwd [%s]\n", pwd);
        printf("filename [%s]\n", filename);
        if(fd == -1)
        {
            write(new_fd, defaulthtml, sizeof(defaulthtml)-1);
            printf("[DATA NOT FOUND] Server Sent 404 not found message to client\n");
        }
        else /* File이 있을 경우 */
        {
            /* SEND FILE HEADER */
            if((strstr(filename, ".jpg")!=NULL)||(strstr(filename, ".jpeg")!=NULL))
                write(new_fd, imageheader, sizeof(imageheader)-1);
            else if(strstr(filename, ".gif")!=NULL||strstr(filename, ".gif")!=NULL)
                write(new_fd, gifheader, sizeof(gifheader)-1);
            else if(strstr(filename, ".pdf")!=NULL||strstr(filename, ".pdf")!=NULL)
                write(new_fd, pdfheader, sizeof(pdfheader)-1);
            else if(strstr(filename, ".mp3")!=NULL||strstr(filename, ".mp3")!=NULL)
                write(new_fd, mp3header, sizeof(mp3header)-1);
            else if(strstr(filename, ".html")!=NULL||strstr(filename, ".html")!=NULL)
                write(new_fd, htmlheader, sizeof(htmlheader)-1);

            /* SEND FILE */
            memset(fbuf, 0x00, sizeof(fbuf));
            while((n=read(fd, fbuf, BUF_SIZE))>0)
                write(new_fd, fbuf, n);

            printf("[SERVER] SENT FILE COMPLETLY : %s\n\n", filename);
        }
        close(fd);
#endif
        close(new_fd);
        if(memcmp(filename, "GOODBYE", strlen("GOODBYE")) == 0)
            break; /* process close */

    }
    close(sockfd);
      
}
