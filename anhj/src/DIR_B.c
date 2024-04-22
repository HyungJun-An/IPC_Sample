#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#include "../include/msg_data.h"
#include <time.h>
#include <sys/timeb.h>

int main(int argc, char *argv[])
{
	if(argc!=2)
	{
	fprintf(stderr,"<USAGE>:DIR_B <msgcount>\n");
	return 0;
	}
	/************************/
	/*시간 구조체 선언		*/
	/************************/
	struct timeb milli_now;
	struct tm *now;
	
        key_t key=12345;
        int msqid;
	int i=0;
        struct message msg;
        msg.msg_type=1;
	int count=atoi(argv[1]);

        //msqid를 얻어옴.
        if((msqid=msgget(key,IPC_CREAT|0666))==-1){
                printf("msgget failed\n");
                exit(0);
        }

        //메시지 보내기 전 msqid_ds를 한번 보자.
	while(i<count)
	{
			ftime(&milli_now);	
			now = localtime(&milli_now.time);
        	sprintf(msg.data.name,"%02d\n",(now->tm_sec*1000)+milli_now.millitm);
        	//메시지를 보낸다.
        	if(msgsnd(msqid,&msg,sizeof(struct real_data),0)==-1)
			{
                		printf("msgsnd failed\n");
                		exit(0);
        	}
		i++;
	}
}
