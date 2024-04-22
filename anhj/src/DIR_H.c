#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#include "../include/msg_data.h"

int main(int argc, char *argv[])
{
	if(argc!=2)
	{
		fprintf(stderr,"USAGE DIR_H <msgcount>\n");
		return 0;
	}

	/****************************/
    /*메세지 큐 세팅            */
    /****************************/
    key_t key=123456;
	key_t skey=1234567;
    int msqid;
    int msqid2;
	int count=atoi(argv[1]);
    struct message msg;
	if((msqid=msgget(key,IPC_CREAT|0666))==-1)
	{
		printf("msgget failed\n");
		exit(0);
	}

	if((msqid2=msgget(skey,IPC_CREAT|0666))==-1)
	{
		printf("msgget failed\n");
		exit(0);
	}
	//메세지를 받는다.
	int i=0;
	while(i<count)
	{
		if(msgrcv(msqid,&msg,sizeof(struct real_data),0,0)==-1)
		{
			printf("msgrcv failed\n");
			continue;
		}
		if(msgsnd(msqid2,&msg,sizeof(struct real_data),0)==-1)
		{
			printf("msgsnd failed\n");
			continue;
		}
	i++;
	}
}
