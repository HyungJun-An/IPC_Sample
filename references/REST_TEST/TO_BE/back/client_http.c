#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <curl/curl.h>

struct memory {
  char *response;
  size_t size;
};

static size_t cb(void *data, size_t size, size_t nmemb, void *clientp)
{
  size_t realsize = size * nmemb;
  struct memory *mem = (struct memory *)clientp;
 
  char *ptr = realloc(mem->response, mem->size + realsize + 1);
  if(!ptr)
    return 0;  /* out of memory! */
 
  mem->response = ptr;
  memcpy(&(mem->response[mem->size]), data, realsize);
  mem->size += realsize;
  mem->response[mem->size] = 0;
 
  return realsize;
}

FILE* fp;
 
int main(int argc, char *argv[])
{
    CURL *curl;
    CURLcode res;

    int i;
    int order_count;
    int stime, rtime;
 
    long status_code = 0;
    struct curl_slist *list = NULL;
    struct memory data;


    if(argc != 2)
    {
        printf("input error : client order_count ex) client 1000\n");
        exit(0);
    }
    order_count = atoi(argv[1]);

    /****************************************************************************/
    /* FOR TEST FILE WRITE                                                      */
    /****************************************************************************/
    fp = fopen ("Latency_HTTP_TEST.csv", "w");
    if(fp == NULL)
    {
        printf("File Open Error\n");
        exit(0);
    }

    /* get a curl handle */
    curl = curl_easy_init();
 

 
    for(i = 0 ; i < order_count; i++)
    {
        memset(&data, 0x00, sizeof(data));
        if(curl) 
        {
    
            /* GET 방식 */
            if(order_count == i+1)
            {
                curl_easy_setopt(curl, CURLOPT_URL, "192.168.1.207:2222/GOODBYE.pdf"); 
            }
            else
            {
                curl_easy_setopt(curl, CURLOPT_URL, "192.168.1.207:2222/helloworld.pdf"); 
            }
          
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&data); /* data send */
            stime = time_check();
            printf("send [%d/%d] \n", i+1, order_count);  
 

            /* Perform the request, res will get the return code */
            res = curl_easy_perform(curl); // curl 실행 res는 curl 실행후 응답내용이 
 
            /* Check for errors */
            if(res == CURLE_OK)
            {
                curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status_code);
                rtime = time_check();
                cal_data_write(stime, rtime);
                printf("recv [%d/%d] \n", i+1, order_count);  
            }
            else
            {
                printf("curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
            }
 
        }
    }
    /* always cleanup */
    curl_easy_cleanup(curl); // curl_easy_init 과 세트
    fclose(fp);
    return 0;
 
}


/*******************************************************************************/
/* 1. 송수신 내용 저장                                                         */
/* 2. 송수신이 걸린 시간 저장                                                  */
/*******************************************************************************/
int cal_data_write(int stime, int rtime)
{
    int interval ;
    fprintf(fp, "%d\n", rtime - stime);

    return 0;
}

/*******************************************************************************/
/* 시간 저장 함수                                                              */
/*******************************************************************************/
int time_check(void)
{
    time_t t;
    struct tm *lt;
    struct timeval tv;
    int cal_time;
    int temp_time;

    if((t = gettimeofday(&tv, NULL)) == -1)
    {
        printf("GETTIMEFDAY() CALL ERROR\n");
    }
    if((lt = localtime(&tv.tv_sec)) == NULL)
    {
        printf("localtime() call error\n");
    }

    temp_time = (lt->tm_hour * 3600) + (lt->tm_min * 60)+ lt->tm_sec;
    cal_time = (temp_time * 1000000)+ tv.tv_usec;


    return cal_time;
}

