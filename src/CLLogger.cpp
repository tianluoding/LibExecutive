#define LOG_FILE_NAME "/home/tld/LibExceutive/CLLogger.txt"
#define MAX_SIZE 256
#define BUFFER_SIZE_LOG_FILE 4096

#include"CLLogger.h"
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<stdio.h>

CLLogger*CLLogger::m_pLog = 0;
CLLogger::CLLogger()
{
    m_fd = open(LOG_FILE_NAME, O_RDWR|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR);
    //printf("%d\n", m_fd);
    //printf("%s\n", strerror(errno));
    m_pLogBuffer = new char[BUFFER_SIZE_LOG_FILE];
    m_nUsedBytesForBuffer = 0;
    
}

CLLogger::~CLLogger()
{
    if(m_fd != -1)
        close(m_fd);
}

CLStatus CLLogger::WriteLog(const char*pstMsg, long lErrorCode)
{
    if(pstMsg == 0)
        return CLStatus(-1, 0);
    if(strlen(pstMsg) == 0)
        return CLStatus(-1, 0);
    if(m_fd == -1)
        return CLStatus(-1, 0);

    if(m_pLogBuffer == 0)
        return CLStatus(-1, 0);
    
    unsigned int nleftroom = BUFFER_SIZE_LOG_FILE - m_nUsedBytesForBuffer;

    unsigned int len_strmsg = strlen(pstMsg);

    char buf[MAX_SIZE];
    snprintf(buf, MAX_SIZE, "Error code: %ld\r\n", lErrorCode);\

    unsigned int len_code = strlen(buf);
    unsigned int total_len = len_code + len_strmsg;

    if((total_len > BUFFER_SIZE_LOG_FILE)||m_bFlagForProcessExit)
    {
        if(m_fd == -1)
            return CLStatus(-1, 0);
        
        CLStatus s = Flush();
        if(!s.IsSuccess())
            return CLStatus(-1, 0);
        
        ssize_t r = write(m_fd, pstMsg, len_strmsg);
        if(r == -1)
            return CLStatus(-1, errno);

        r = write(m_fd, buf, len_code);
        if(r == -1)
            return CLStatus(-1, errno);

        return CLStatus(0, 0);
    }

    if(total_len > nleftroom)
    {
        CLStatus s = Flush();
        if(!s.IsSuccess())
            return CLStatus(-1, 0);
    }

    memcpy(m_pLogBuffer + m_nUsedBytesForBuffer, pstMsg, len_strmsg);

    m_nUsedBytesForBuffer += len_strmsg;

    memcpy(m_pLogBuffer + m_nUsedBytesForBuffer, buf, len_code);

    m_nUsedBytesForBuffer += len_code;

    return CLStatus(0, 0);
}

CLLogger*CLLogger::GetInstance()
{
    if(m_pLog == 0)
    {
        m_pLog = new CLLogger;

        if(atexit(CLLogger::OnProcessExit) != 0)
        {
            if(m_pLog != 0)
            {
                m_pLog->WriteLog("In CLLogger::GetInstance(), atexit error", errno);
                m_pLog->Flush();
            }
        }
        
    }
    return m_pLog;
}

CLStatus CLLogger::WriteLogMsg(const char* pstrMsg, long lErrorCode)
{
    CLLogger*pLog = CLLogger::GetInstance();
    //printf("%d", pLog->m_fd);
    if(pLog == 0)
        return CLStatus(-1, 0);
    
    CLStatus s = pLog->WriteLog(pstrMsg, lErrorCode);
    if(s.IsSuccess())
        return CLStatus(0, 0);
    else
        return CLStatus(-1, 0);
}

CLStatus CLLogger::Flush()
{
    if(m_fd == -1)
        return CLStatus(-1, 0);
    
    if(m_pLogBuffer == 0)
        return CLStatus(-1, 0);

    if(m_nUsedBytesForBuffer == 0)
        return CLStatus(0, 0);

    ssize_t r = write(m_fd, m_pLogBuffer, m_nUsedBytesForBuffer);
    if(r == -1)
    {
        return CLStatus(-1, errno);
    }

    m_nUsedBytesForBuffer = 0;

    return CLStatus(0, 0);
}

void CLLogger::OnProcessExit()
{
    CLLogger* pLogger = CLLogger::GetInstance();
    if(pLogger != 0)
    {
        pLogger->Flush();
        pLogger->m_bFlagForProcessExit = true;
    }
}