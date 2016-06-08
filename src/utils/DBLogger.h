#ifndef __DB_LOGGER__
#define __DB_LOGGER__
#include <string>
#include <database/mysql/command.h>
#include <memory>

using namespace std;

class DBLogger
{
	unique_ptr<command>	m_cmd;
	bool				m_active;
	DBLogger();
	~DBLogger()	{}
public:
	void	Active()	{m_active = true;}	
	void	Deactive()	{m_active = false;}
	void	LD_LogEvent(int EventID, string EventInfo, int EventStatus);

	static DBLogger* singleton()	{static DBLogger s_dbLogger; return  &s_dbLogger;}
};

#endif