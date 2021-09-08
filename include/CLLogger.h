#ifndef _CLLOGGER_H
#define _CLLOGGER_H

#include"CLStatus.h"

class CLLogger
{
public:
    static CLLogger* GetInstance();
    static CLStatus WriteLogMsg(const char*pstrMsg, long lErrorCode);
    CLStatus WriteLog(const char*pstMsg, long lErrorCode);
    CLStatus Flush();//flush buffer
private:
    CLLogger();
    ~CLLogger();

    CLLogger(const CLLogger& c);//not use
    CLLogger& operator=(const CLLogger& c);//not use

    static void OnProcessExit();
private:
    int m_fd;
    static CLLogger* m_pLog;

    char* m_pLogBuffer;
    unsigned int m_nUsedBytesForBuffer;

    bool m_bFlagForProcessExit;
};

#endif