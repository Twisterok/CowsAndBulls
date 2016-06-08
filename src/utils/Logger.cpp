#include <windows.h>
#include "logger.h"
#include <stdio.h>
#include "DBLogger.h"

Logger::Logger()
{
}
//---------------------------------------------
Logger::~Logger()
{
	doneLog();
}

//---------------------------------------------
bool Logger::doLogFile(int LogType)
{
	return (LogType <= m_logType);
}

//---------------------------------------------
void Logger::logWrite(string str, int LogType, int Err)
{
    if (!m_logFile)     return;
	logWriteNoCrLf(str, LogType, Err);

	if (LogType <= m_logType)	m_logFile << endl;
}


//---------------------------------------------
void Logger::logWriteNoCrLf(string s, int LogType, int Err)
{
	SYSTEMTIME time;
	char TimeBuf[64];
	GetLocalTime(&time);

	if (LogType <= m_logType)
	{
		m_mutex.lock();
		sprintf_s(TimeBuf, "%02d.%02d.%04d %02d:%02d:%02d.%04d %08x ", time.wDay, time.wMonth, time.wYear, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, GetCurrentThreadId());
		m_logFile << TimeBuf << s;
		m_mutex.unlock();
	}
}

//---------------------------------------------
void Logger::initLog(string LogName, int LogType)
{
	if (m_logFile)		m_logFile.close();
	
	m_logFile.open(LogName,ios::app);
	if (!m_logFile)		throw runtime_error("Cant open " + LogName);
  
	m_logType = LogType;
}


//---------------------------------------------
void Logger::doneLog()
{
	if (m_logFile)	m_logFile.close();
}


//---------------------------------------------
Logger* Logger::singleton()
{
	static Logger s_logger;
	return &s_logger; 
}