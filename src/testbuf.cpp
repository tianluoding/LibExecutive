#include"CLLogger.h"
#include"CLStatus.h"
#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<iostream>

using namespace std;

int main()
{
    const int n = 1000000;

    struct timeval tv;
    gettimeofday(&tv, 0);

    for(int i = 0; i < n; i++)
        CLLogger::WriteLogMsg("nihao", 0);

    CLLogger*pLog = CLLogger::GetInstance();
    pLog->Flush();

    struct timeval tv1;
    gettimeofday(&tv1, 0);

    long t = n*(tv1.tv_sec - tv.tv_sec) + (tv1.tv_usec - tv.tv_usec);
    cout<<t<<endl;

    return 0;
}