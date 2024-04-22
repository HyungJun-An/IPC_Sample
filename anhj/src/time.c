#include <stdio.h>
#include <time.h>
#include <sys/timeb.h>

int main(void)
{

    struct timeb milli_now;
    struct tm *now;

    ftime(&milli_now);
    now = localtime(&milli_now.time);
    
    printf("[%04d/%02d/%02d] %02d:%02d:%02d (%02dmsec)\n", 1900 + now->tm_year, 
            now->tm_mon + 1, now->tm_mday, now->tm_hour, 
            now->tm_min, now->tm_sec, milli_now.millitm);

	return 0;
}
