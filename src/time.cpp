#include <gtest/gtest.h>
#include <iostream>
#include <time.h>
#include <sys/time.h>

//unix内核提供的时间都是从协调世界时(UTC)1970年1月1日开始，到现在的秒数.
//这个秒数是用time_t表示的

/*
                +--------+                                                                           +--------------------------------------+
                | string |                                                                           |            format string             |
                +--------+                                                                           +--------------------------------------+
                  |                                                                                    ^              ^
                  |                                                                                    | strftime_l   | strftime
                  |                                                                                    |              |
                  |        strptime        +---------------------------------------------+             |              |
                  +----------------------> |                                             | ------------+              |
                                           |                  struct tm                  |                            |
                                           |               (structed time)               |                            |
                                           |                                             | ---------------------------+
                                           +---------------------------------------------+
                                             |              ^                 ^
                                             | mktime       | gmtime          | localtime
                                             v              |                 |
+---------+                                +---------------------------------------------+                                       +----------+
| timeval |                tv_sec          |                   time_t                    |  tv_sec                               | timespec |
|         | .............................> |               (calendar time)               | <.................................... |          |
+---------+                                +---------------------------------------------+                                       +----------+
  ^                                          ^                                                                                     ^
  |                                          | time                                                                                |
  |                                          |                                                                                     |
  |                        gettimeofday    +-------------+  clock_gettime                                                          |
  +--------------------------------------- | system core | ------------------------------------------------------------------------+
                                           +-------------+


*/
TEST(time,time)
{
    //返回系统当前时间
    time_t t = time(NULL);
    std::cout << t << std::endl;
}

TEST(time,clock_gettime)
{
    //clock_gettime可以获取各种时钟的时间

    clockid_t clock_id;
    struct timespec tsp;

    //CLOCK_REALTIME:系统的真正时间，可以精确到纳秒，比time函数精度高一些
    clock_gettime(CLOCK_REALTIME,&tsp);
    std::cout << tsp.tv_sec << ":" << tsp.tv_nsec << std::endl;

    //CLOCK_REALTIME_COARSE:CLOCK_REALTIME的糙快猛版本
    clock_gettime(CLOCK_REALTIME_COARSE,&tsp);
    std::cout << tsp.tv_sec << ":" << tsp.tv_nsec << std::endl; 

    //CLOCK_MONOTONIC
    clock_gettime(CLOCK_MONOTONIC,&tsp);
    std::cout << tsp.tv_sec << ":" << tsp.tv_nsec << std::endl; 

    //sleep(2);
    //CLOCK_PROCESS_CPUTIME_ID,高精度进程的CPU时间
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&tsp);
    std::cout << tsp.tv_sec << ":" << tsp.tv_nsec << std::endl; 

    //CLOCK_THREAD_CPUTIME_ID,线程CPU时间
    clock_gettime(CLOCK_THREAD_CPUTIME_ID,&tsp);
    std::cout << tsp.tv_sec << ":" << tsp.tv_nsec << std::endl; 
}

TEST(time,gettimeofday)
{
    //gettimeofday也是获取系统的当前时间，它比time精度高一些，精确到微妙
    //它的第二个参数只能穿NULL，传别的都非法  很操蛋
    //返回值：总是返回0  也很操蛋
    struct timeval tv = {0};
    gettimeofday(&tv,NULL);
    std::cout << tv.tv_sec << ":" << tv.tv_usec << std::endl; 
}

TEST(time,gmtime_localtime)
{
    time_t t = time(NULL);
    struct tm *gmt = gmtime(&t);
    struct tm *lct = localtime(&t);
    std::cout << gmt->tm_year + 1900 << "-" << gmt->tm_mon + 1 << "-" << gmt->tm_mday << " "
              << gmt->tm_hour << ":" << gmt->tm_min << ":" << gmt->tm_sec << std::endl;

    std::cout << lct->tm_year + 1900 << "-" << lct->tm_mon + 1 << "-" << lct->tm_mday << " "
              << lct->tm_hour << ":" << lct->tm_min << ":" << lct->tm_sec << std::endl;
}

TEST(time, clock)
{
    time_t c1 = clock();
    getchar();
    time_t c2 = clock();
    getchar();
    time_t c3 = clock();

    std::cout << c1 << " " << c2 << " " << c3 << " " << std::endl;
    std::cout << c1 / CLOCKS_PER_SEC << " " << c2 / CLOCKS_PER_SEC << " " << c3 / CLOCKS_PER_SEC << " " << std::endl;
}
