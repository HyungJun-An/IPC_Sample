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
		fprintf(stderr,"USAGE DIR_H <msgcount>\n");
		return 0;
	}
	/****************************/
	/*시간 구조체 선언			*/
	/****************************/
	struct timeb milli_now;
	struct tm *now;
	/****************************/
    /*메세지 큐 세팅            */
    /****************************/
	key_t key=1234567;
    int msqid;
	int count=atoi(argv[1]);
    struct message msg;
	if((msqid=msgget(key,IPC_CREAT|0666))==-1)
	{
		printf("msgget failed\n");
		exit(0);
	}

	int i=0;
	int sendtime, delay , first, last, recvtime;
	int result;
	while(i<count)
	{
		if(msgrcv(msqid,&msg,sizeof(struct real_data),0,0)==-1)
		{
			printf("msgrcv failed\n");
			continue;
		}
		if(msg.data.name==NULL)
		{
			continue;
		}
		ftime(&milli_now);
		now= localtime(&milli_now.time);	
		sendtime=atoi(msg.data.name);
		recvtime=(now->tm_sec*1000)+milli_now.millitm;
		if(i==0)
		{
       		printf("[first]\n %s",msg.data.name);
			if(recvtime<sendtime)
			{
				delay=recvtime-sendtime;
			}
			else
			{
				delay=recvtime-sendtime;
			}
		}
		if(i==count-1)
		{
       		printf("[last]\n%02d.%02d\n",now->tm_sec,milli_now.millitm);
			if(recvtime<sendtime)
			{
				delay=recvtime-sendtime;
			}
			else
			{
				delay=recvtime-sendtime;
			}

		}
		if(recvtime-sendtime==1000)
		{
			printf("1sec/%d\n",i);
		}

		if(recvtime<sendtime)
		{
			delay=recvtime-sendtime;
		}
		else
		{
			delay=recvtime-sendtime;
		}
	result+=delay;
	i++;
	}
	printf("delay=%d\n result=%d\n sendtime=%d\n recvtime=%d\n",delay,result,sendtime,recvtime);
}
