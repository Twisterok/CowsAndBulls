#ifndef __LOGGER_H__
#define __LOGGER_H__
#include <string>
#include <fstream>
#include <boost/thread/mutex.hpp>

using namespace std;

class Logger
{
	ofstream	    m_logFile;
	int			    m_logType;
	boost::mutex    m_mutex;
	Logger();
	~Logger();
public:
	void initLog(string LogName, int LogType);
	void doneLog();
	void logWrite(string s, int LogType, int Err);
	void logWriteNoCrLf(string s, int LogType, int Err);
	bool doLogFile(int LogType);

	static Logger*	singleton();
};

#endif